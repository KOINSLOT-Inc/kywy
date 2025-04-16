<!--
SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.

SPDX-License-Identifier: GPL-3.0-or-later
-->

# Contributing

## Local Development Workflow

### Easy Option: Arduino IDE

We suggested using the Arduino IDE for local development.

To do this, first clone this repo either directly into the `Arduino/libraries` folder on your machine, or elsewhere and
symlink the repo into the `Arduino/libraries` folder.

Next, create a `.development` file and a `src/src.ino` file (these are `.gitignore`'d). Per [this
thread](https://arduino.stackexchange.com/a/43314), the `.development` file will let the Arduino IDE save over library
files and the `src/src.ino` file will let you open then whole library in the IDE by opening that file.

Now you can edit any library file in the IDE and test changes manually via the `src/src.ino` file. You can also use the
IDE for formatting files (`<cmd/ctrl-T>`) and the IDE's built in LSP for easy code completion.

You are also welcome to use whatever text editor you prefer for editing library files. We recommend the IDE here only
because it requires the fewest external dependencies/installs to get editor tooling.

#### Uploads

Uploads can be done using the Arduino IDE natively.

### Flexible Option: Arduino LSP

You can also install the
[arduino-language-server](https://github.com/arduino/arduino-language-server?tab=readme-ov-file#usage) locally and
configure it with your IDE so that you can use any editor you want.

E.g. for Neovim you might add

```lua
if vim.fn.executable('arduino-language-server') == 1 then
  require('lspconfig').arduino_language_server.setup {
    cmd = {
        "arduino-language-server",
        "-cli-config", "~/arduino-cli.yaml",
        "-fqbn", "arduino:mbed_rp2040:pico",
    }
  }
end
```

and a local `.nvim.lua` file like

```lua
vim.filetype.add({
  extension = {
    cpp = 'arduino',
    hpp = 'arduino',
  }
})
```

so that `hpp` and `cpp` files are recognized as `arduino` files instead of `cpp` files.

#### Uploads

Uploads can be done using the `arduino-cli`. A Make target, `make upload/examples/<example>`, is available to quickly
upload any project in the `examples/` directory.

## Releases

Tagging and releases are automatically managed by workflows and happen every time a PR is made.

Tagging is managed by: https://github.com/anothrNick/github-tag-action

Notes:
* defaults to a minor bump
* can select major, minor, patch, or no bump by including `#major`, `#minor`, `#patch`, or `#none` respectively in a
  commit message.

Releases created automatically after tags.
