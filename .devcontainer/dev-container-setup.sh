#!/bin/bash

# SPDX-FileCopyrightText: 2025 2025 KOINSLOT Inc.
#
# SPDX-License-Identifier: GPL-3.0-or-later

# Setup environment and add scripts to path
chmod +x .devcontainer/scripts/*
echo 'export PATH="$PATH:/workspaces/kywy/.devcontainer/scripts"' >> ~/.bashrc
echo 'export PATH="$PATH:/workspaces/kywy/.devcontainer/scripts"' >> ~/.zshrc

# Welcome message
chmod +x .devcontainer/welcome-message.sh
echo 'source /workspaces/kywy/.devcontainer/welcome-message.sh' >> ~/.bashrc
echo 'source /workspaces/kywy/.devcontainer/welcome-message.sh' >> ~/.zshrc


# Install missing dependencies
# sudo apt update # uncomment if we need apt installs
pip install reuse
pip install pipenv

# Clean up to save space
sudo apt-get clean
sudo rm -rf /var/lib/apt/lists/*
pip cache purge

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
