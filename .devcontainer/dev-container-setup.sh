#!/bin/bash

# SPDX-FileCopyrightText: 2025 KOINSLOT Inc.
#
# SPDX-License-Identifier: GPL-3.0-or-later

# Setup environment and add scripts to path
chmod +x .devcontainer/scripts/*
echo 'export PATH="$PATH:/workspaces/kywy/scripts"' >> ~/.bashrc
echo 'export PATH="$PATH:/workspaces/kywy/scripts"' >> ~/.zshrc

# Welcome message
chmod +x .devcontainer/welcome-message.sh
echo 'source /workspaces/kywy/.devcontainer/welcome-message.sh' >> ~/.bashrc
echo 'source /workspaces/kywy/.devcontainer/welcome-message.sh' >> ~/.zshrc


# Parse args
SKIP_MAKE=0
for arg in "$@"; do
  case "$arg" in
    --skip-make)
      SKIP_MAKE=1
      ;;
  esac
done

# Install from makefile unless skipped
if [ "$SKIP_MAKE" -ne 1 ]; then
  echo "Running make install (this may install Python deps and can be large)..."
  if make install; then
    echo "make install finished"
  else
    echo "make install failed or was skipped"
  fi
else
  echo "Skipping make install (invoked with --skip-make)"
fi

# Clean up to save space (best-effort, non-fatal)
set +e
if command -v apt-get >/dev/null 2>&1; then
  sudo apt-get clean || true
  sudo rm -rf /var/lib/apt/lists/* || true
fi
if command -v pip >/dev/null 2>&1; then
  pip cache purge || true
fi
set -e

# Install arduino-cli directly from arduino
if ! command -v arduino-cli &> /dev/null; then
  echo "Installing arduino-cli..."
  cd ~
  curl -fsSL https://downloads.arduino.cc/arduino-cli/arduino-cli_latest_Linux_64bit.tar.gz -o arduino-cli.tar.gz
  tar -xzf arduino-cli.tar.gz
  sudo mv arduino-cli /usr/local/bin/
  rm arduino-cli.tar.gz
  rm LICENSE.txt
else
  echo "arduino-cli already installed"
fi

# Configure additional library path for Kywy sources
arduino-cli config init || true
arduino-cli core update-index || true
arduino-cli config set directories.user /workspaces/kywy/.arduino
arduino-cli config set directories.data /workspaces/kywy/.arduino/data
arduino-cli config set directories.downloads /workspaces/kywy/.arduino/staging
arduino-cli config set directories.builtin.libraries /workspaces/kywy/
arduino-cli config set build_cache.path /workspaces/kywy/.cache
arduino-cli core install arduino:mbed_rp2040 || true

# Remove unused SDKs and preinstalled junk
sudo rm -rf /usr/share/dotnet
sudo rm -rf ~/.dotnet
sudo rm -rf /usr/local/lib/node_modules
sudo rm -rf /usr/local/bin/node /usr/local/bin/npm
rm -rf ~/.npm
sudo apt-get remove -y docker-ce docker-ce-cli containerd.io
sudo rm -rf /usr/share/doc/*
sudo apt-get clean
rm -rf ~/.cache
sudo apt-get autoremove -y
sudo apt-get autoclean

# Let user know we are ready
echo "Container Setup Complete"
