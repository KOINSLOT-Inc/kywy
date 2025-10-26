#!/usr/bin/env python3
"""
async multi-programmer.py

Async, cross-platform multi-UF2 uploader.

Behavior:
- Single-threaded reset/detection loop (sequentially touches serial ports at 1200 baud).
- Asynchronous concurrent copy tasks (bounded by --threads semaphore) perform UF2 file copies without blocking detection.
- Watch mode: keep running and program devices as they're plugged in. One-shot mode: run a single pass and exit.

Requirements: Python 3.8+, pyserial
"""

from __future__ import annotations

import argparse
import asyncio
import logging
import os
import platform
import shutil
import sys
import time
from typing import Dict, List, Optional, Set

try:
    import serial
    import serial.tools.list_ports as list_ports
except Exception:  # pragma: no cover - dependency check
    serial = None
    list_ports = None

LOGGER = logging.getLogger("multi-programmer")


def require_pyserial_or_die() -> None:
    if serial is None or list_ports is None:
        print("pyserial is required. Install with: pip install pyserial")
        sys.exit(2)


def list_serial_ports() -> List[str]:
    require_pyserial_or_die()
    ports: List[str] = []
    excluded: List[str] = []
    for p in list_ports.comports():
        dev = p.device
        if not dev:
            continue
        if dev.startswith("/dev/ttyS"):
            excluded.append(dev)
            continue
        if getattr(p, "vid", None) is not None or "ACM" in dev or "USB" in dev or "ttyUSB" in dev or dev.startswith("COM") or dev.startswith("/dev/cu."):
            ports.append(dev)
        else:
            desc = (getattr(p, "description", "") or "").upper()
            if "USB" in desc or "CDC" in desc or "SERIAL" in desc:
                ports.append(dev)
            else:
                excluded.append(dev)

    LOGGER.debug("Detected serial ports: included=%s excluded=%s", ports, excluded)
    return ports


def snapshot_mounts() -> Set[str]:
    system = platform.system()
    mounts: Set[str] = set()
    if system == "Linux":
        try:
            with open("/proc/mounts", "r", encoding="utf-8") as f:
                for line in f:
                    parts = line.split()
                    if len(parts) < 3:
                        continue
                    device, mnt, fstype = parts[0], parts[1], parts[2]
                    if fstype.lower() in ("vfat", "fat", "msdos", "exfat", "fuseblk") or mnt.startswith("/media") or mnt.startswith("/run/media") or mnt.startswith("/mnt"):
                        mounts.add(mnt)
        except FileNotFoundError:
            for base in ("/media", "/run/media", "/mnt"):
                if os.path.isdir(base):
                    for n in os.listdir(base):
                        mounts.add(os.path.join(base, n))
    elif system == "Darwin":
        vols = "/Volumes"
        if os.path.isdir(vols):
            for name in os.listdir(vols):
                mounts.add(os.path.join(vols, name))
    elif system == "Windows":
        try:
            import ctypes

            DRIVE_REMOVABLE = 2
            kernel32 = ctypes.windll.kernel32
            bitmask = kernel32.GetLogicalDrives()
            for i in range(26):
                if bitmask & (1 << i):
                    drive = f"{chr(65 + i)}:/"
                    dtype = kernel32.GetDriveTypeW(ctypes.c_wchar_p(drive))
                    if dtype in (DRIVE_REMOVABLE, 3):
                        mounts.add(drive)
        except Exception:
            for letter in "ABCDEFGHIJKLMNOPQRSTUVWXYZ":
                path = f"{letter}:/"
                if os.path.exists(path):
                    mounts.add(path)
    else:
        for base in ("/media", "/mnt", "/Volumes"):
            if os.path.isdir(base):
                for name in os.listdir(base):
                    mounts.add(os.path.join(base, name))

    return mounts


def wait_for_new_mount(before: Set[str], timeout: float = 20.0, poll: float = 0.5) -> Optional[str]:
    end = time.time() + timeout
    known_tokens = ("RPI", "RP2", "PICO", "UF2", "MBED", "MICROBIT")
    while time.time() < end:
        now = snapshot_mounts()
        new = now - before
        if new:
            for candidate in sorted(new):
                base = os.path.basename(candidate).upper()
                if any(tok in base for tok in known_tokens):
                    LOGGER.debug("Detected new mount (preferred): %s", candidate)
                    return candidate
            chosen = sorted(new)[0]
            LOGGER.debug("Detected new mount: %s", chosen)
            return chosen
        time.sleep(poll)
    return None


def touch_1200_and_wait_for_mount(port: str, timeout: float = 20.0) -> Optional[str]:
    """Blocking: touch serial port at 1200 baud and wait for a new mount (blocking)."""
    LOGGER.info("%s: snapshotting mounts before touch", port)
    before = snapshot_mounts()
    try:
        require_pyserial_or_die()
        LOGGER.info("%s: touching at 1200 baud", port)
        try:
            with serial.Serial(port, 1200, timeout=0.1) as s:
                time.sleep(0.05)
            time.sleep(0.15)
        except Exception as e:
            LOGGER.warning("%s: failed to open serial port for 1200 touch: %s", port, e)
            return None
    except SystemExit:
        return None

    LOGGER.info("%s: waiting for new mount (timeout=%ss)", port, timeout)
    return wait_for_new_mount(before, timeout=timeout)


def copy_file_blocking(src: str, dst: str) -> None:
    """Robust blocking copy: copy file in chunks, fsync, and try to preserve metadata.

    This avoids shutil.copy2 raising FileNotFoundError during copystat if the
    destination disappears mid-copy (common when the USB drive is ejected).
    """
    dst_dir = os.path.dirname(dst)
    if not dst_dir:
        raise ValueError("Destination has no directory: %r" % dst)
    if not os.path.isdir(dst_dir):
        raise FileNotFoundError("Destination directory does not exist: %r" % dst_dir)

    # Attempt the copy with a couple of retries in case the mount briefly disappears
    attempts = 3
    for attempt in range(1, attempts + 1):
        try:
            # Stream copy to avoid reading entire file into memory
            with open(src, "rb") as fsrc:
                # Use a temporary filename in the same mount to avoid partial-file races
                tmp_dst = dst + ".tmp"
                with open(tmp_dst, "wb") as fdst:
                    shutil.copyfileobj(fsrc, fdst, length=1024 * 1024)
                    fdst.flush()
                    try:
                        os.fsync(fdst.fileno())
                    except Exception:
                        # fsync may fail on some filesystems/devices; ignore but log
                        LOGGER.debug("fsync failed for %s (attempt %d)", tmp_dst, attempt)

            # Try to move tmp to final dst atomically
            try:
                os.replace(tmp_dst, dst)
            except Exception:
                # Fall back to rename (may fail on some platforms) then raise if needed
                try:
                    os.rename(tmp_dst, dst)
                except Exception:
                    # If destination disappeared between copy and rename, raise to retry
                    LOGGER.debug("rename/replace failed for %s -> %s (attempt %d)", tmp_dst, dst, attempt)
                    raise

            # Attempt to copy metadata; ignore errors if the filesystem doesn't support it
            try:
                shutil.copystat(src, dst)
            except Exception:
                LOGGER.debug("copystat failed for %s -> %s", src, dst)

            # Success
            return
        except FileNotFoundError as e:
            LOGGER.warning("copy attempt %d/%d failed with FileNotFoundError: %s", attempt, attempts, e)
            # Small backoff before retrying
            time.sleep(0.2)
            continue
        except Exception:
            LOGGER.exception("Unexpected error during copy attempt %d/%d", attempt, attempts)
            # For unexpected errors, don't retry indefinitely
            raise

    # If we reach here, all attempts failed
    raise FileNotFoundError(f"Failed to copy {src} to {dst} after {attempts} attempts")


async def main_async(argv: Optional[List[str]] = None) -> int:
    parser = argparse.ArgumentParser(description="Async multi-threaded UF2 programmer using 1200-baud touch")
    parser.add_argument("uf2", help="Path to the .uf2 file to flash")
    parser.add_argument("--ports", nargs="*", help="Serial ports to try (default: auto-list all COM/TTY ports)")
    parser.add_argument("--timeout", type=float, default=20.0, help="Timeout in seconds waiting for mount after touch (default: 20)")
    parser.add_argument("--threads", type=int, default=8, help="Maximum parallel copy tasks (default: 8)")
    parser.add_argument("--verbose", "-v", action="store_true", help="Verbose logging")
    parser.add_argument("--no-watch", dest="watch", action="store_false", help="Run once and exit after initial programming (default: keep running and watch for new devices)")
    parser.add_argument("--cooldown", type=float, default=60.0, help="Seconds to wait before trying the same port again (default: 60s)")

    args = parser.parse_args(argv)

    logging.basicConfig(level=logging.DEBUG if args.verbose else logging.INFO, format="%(asctime)s %(levelname)s %(message)s")

    uf2_path = os.path.abspath(args.uf2)
    if not os.path.isfile(uf2_path):
        LOGGER.error("UF2 file not found: %s", uf2_path)
        return 3

    semaphore = asyncio.Semaphore(max(1, args.threads))
    active: Dict[str, asyncio.Task] = {}
    last_attempt: Dict[str, float] = {}
    results: Dict[str, bool] = {}
    seen_mounts: Set[str] = set()
    # map serial port -> active mount path (when a port triggered a mount and a copy is in progress)
    port_to_active_mount: Dict[str, str] = {}
    # ports that have been programmed successfully and should not be touched again until they disappear
    programmed_ports: Set[str] = set()

    async def copy_coro(mount: str, port: Optional[str] = None) -> None:
        dst = os.path.join(mount, os.path.basename(uf2_path))
        who = port or mount
        try:
            LOGGER.info("%s: waiting for copy semaphore", who)
            await semaphore.acquire()
            LOGGER.info("%s: starting copy %s -> %s", who, uf2_path, dst)
            await asyncio.to_thread(copy_file_blocking, uf2_path, dst)
            LOGGER.info("%s: copy complete", who)
            results[mount] = True
            # If this copy was triggered by a serial port, mark that port as programmed
            if port:
                programmed_ports.add(port)
        except Exception:
            LOGGER.exception("%s: copy failed", who)
            results[mount] = False
        finally:
            try:
                semaphore.release()
            except Exception:
                pass
            last_attempt[mount] = time.time()
            # cleanup port->mount mapping if present
            if port and port in port_to_active_mount:
                try:
                    del port_to_active_mount[port]
                except KeyError:
                    pass
            active.pop(mount, None)

    async def reset_once() -> None:
        if args.ports and len(args.ports) > 0:
            ports_iter = list(args.ports)
        else:
            try:
                ports_iter = list_serial_ports()
            except Exception:
                LOGGER.exception("Failed to list serial ports in reset pass")
                ports_iter = []

        # detect any newly-mounted RPI-like UF2 drives and schedule copies for them immediately
        try:
            current_mounts = snapshot_mounts()
            # prune seen_mounts for mounts that disappeared
            seen_mounts.intersection_update(current_mounts)
            new_mounts = current_mounts - seen_mounts
            for m in sorted(new_mounts):
                base = os.path.basename(m).upper()
                if "RPI" in base or "RP2" in base or "RPI-RP2" in base or "PICO" in base or "UF2" in base:
                    if m not in active:
                        LOGGER.info("Auto-detected UF2 mount %s (basename=%s) -> scheduling copy", m, base)
                        task = asyncio.create_task(copy_coro(m))
                        active[m] = task
                    seen_mounts.add(m)
        except Exception:
            LOGGER.exception("Error scanning mounts for auto-detect")

        LOGGER.debug("reset_once: start")

        # prune programmed_ports for ports that are gone
        try:
            current_ports_list = list_serial_ports()
        except Exception:
            current_ports_list = []
        for p in list(programmed_ports):
            if p not in current_ports_list:
                programmed_ports.discard(p)

        for port in ports_iter:
            now = time.time()
            # skip this port if we've already programmed it successfully and it hasn't disappeared yet
            if port in programmed_ports:
                LOGGER.debug("Port %s was programmed; skipping reset until it disappears", port)
                continue
            # skip if this port already has an active mount copy in progress
            if port in port_to_active_mount:
                LOGGER.debug("Port %s has active mount %s; skipping reset", port, port_to_active_mount.get(port))
                continue
            if now - last_attempt.get(port, 0.0) < args.cooldown:
                LOGGER.debug("Port %s in cooldown; skipping reset", port)
                continue

            mount = await asyncio.to_thread(touch_1200_and_wait_for_mount, port, args.timeout)
            if mount:
                LOGGER.info("%s: detected mount %s -> scheduling copy", port, mount)
                # record that this port caused the mount so we can avoid touching it again
                port_to_active_mount[port] = mount
                task = asyncio.create_task(copy_coro(mount, port))
                active[mount] = task
            else:
                last_attempt[port] = time.time()
        LOGGER.debug("reset_once: end")

    async def reset_loop() -> None:
        while True:
            await reset_once()
            await asyncio.sleep(0.6)

    # Run either watch mode or one-shot
    if args.watch:
        LOGGER.info("Starting in watch mode; uf2=%s", uf2_path)
        reset_task = asyncio.create_task(reset_loop())

        async def monitor_active() -> None:
            try:
                while True:
                    # prune finished tasks
                    for k, t in list(active.items()):
                        if t.done():
                            active.pop(k, None)
                    await asyncio.sleep(0.8)
            except asyncio.CancelledError:
                LOGGER.debug("monitor_active cancelled")

        monitor_task = asyncio.create_task(monitor_active())
        try:
            # wait forever until cancelled by KeyboardInterrupt
            await asyncio.gather(reset_task, monitor_task)
        except KeyboardInterrupt:
            LOGGER.info("Interrupted; cancelling tasks")
            reset_task.cancel()
            monitor_task.cancel()
            for t in list(active.values()):
                t.cancel()
            await asyncio.gather(reset_task, monitor_task, *active.values(), return_exceptions=True)
    else:
        LOGGER.info("Running one-shot pass; uf2=%s", uf2_path)
        await reset_once()
        # wait for copies to finish
        if active:
            await asyncio.gather(*active.values(), return_exceptions=True)

    success = [p for p, ok in results.items() if ok]
    failed = [p for p, ok in results.items() if not ok]
    LOGGER.info("Summary: success=%s failed=%s", success, failed)
    return 0 if not failed else 5


def main(argv: Optional[List[str]] = None) -> int:
    return asyncio.run(main_async(argv))


if __name__ == "__main__":
    raise SystemExit(main())