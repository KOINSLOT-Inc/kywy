# SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
#
# SPDX-License-Identifier: GPL-3.0-or-later


def test_app(dut):
    dut.expect_unity_test_output(timeout=240)
