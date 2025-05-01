---
hide:
  - footer
---

<!--
SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.

SPDX-License-Identifier: GPL-3.0-or-later
-->

# Debug Tools

## Click-to-Tick

Things moving too fast to easily debug? The Click-to-Tick feature lets you control the clock via the Serial Monitor in
the Arduino IDE.

To enable Click-to-Tick, specify it in the `EngineOptions` when starting the engine:

```c++
engine.start(Kywy::EngineOptions().clickToTick(true));
```

After doing this the clock will not send `TICK` events unless you send some input in the Serial Monitor (click the
Serial Monitor button in the top right, click on the text box in the Serial Monitor at the bottom, type a message, hit
Enter to send).

The commands available are:

```
`tick` - causes the clock to send 1 `TICK` event
```
