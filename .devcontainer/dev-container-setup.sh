#!/bin/bash

# SPDX-FileCopyrightText: 2025 2025 KOINSLOT Inc.
#
# SPDX-License-Identifier: GPL-3.0-or-later

# Setup environment and add scripts to path
chmod +x .devcontainer/scripts/*.sh
echo 'export PATH="$PATH:/workspaces/kywy-rust/.devcontainer/scripts"' >> ~/.bashrc
echo 'export PATH="$PATH:/workspaces/kywy-rust/.devcontainer/scripts"' >> ~/.zshrc

# Install missing dependencies
sudo apt update
sudo apt install -y arduino-cli

# Clean up to save space
sudo apt-get clean
sudo rm -rf /var/lib/apt/lists/*
pip cache purge

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