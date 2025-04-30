---
hide:
    - footer
---

<!--
SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.

SPDX-License-Identifier: GPL-3.0-or-later
-->

# Troubleshooting
## Common Issues
### 1. Arduino can not find the Kywy
#### Bad USB cable
This is a common cause of the board not being found. Make sure you are
using a data cable, not just a power cable. Try a different cable and verify
that it works with a different device. If you can verify your cable works and
something is wrong with the board, please contact us.

#### Hard reset the board
It is possible to upload a broken sketch or a sketch that does not start the
serial port. To fix this follow the following steps to hard reset and put the
Kywy into programming mode:
1. Disconnect the USB and turn the Kywy off.
2. Using a paperclip or similar small pointy thing, press the reset button on the
back of the Kywy, to the right of the Koinslot logo.
3. While holding the reset button, connect the USB cable to the Kywy or power it on.
4. Wait a few second and the device should show up as a USB drive.

### 2. Arduino IDE can not find the library
When uploading, if you get the message "Compilation error:
Kywy1.hpp: No such file or directory", it means that the library is not
installed.
On the arduino IDE, open the library manager and search for "Kywy". Install the
library and restart the IDE.

## Contact us:
For help use the discord:

<a href="https://discord.gg/zAYym57Fy6"
    ><img
        alt="Discord"
        src="https://img.shields.io/discord/1172988360063725629?style=for-the-badge&logo=discord"
/></a>

You can submit issues with the Arduino library on GitHub:
[kywy issues GitHub](https://github.com/KOINSLOT-Inc/kywy/issues)
