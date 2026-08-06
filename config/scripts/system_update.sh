#!/bin/bash

set -e

echo " Starting Full Gentoo System Update..."

echo "1. Syncing repository and updating entire system (World update)..."
sudo emerge --sync
sudo emerge --update --deep --newuse --with-bdeps=y @world

echo "2. Rebuilding external kernel modules (e.g., NVIDIA drivers)..."
sudo emerge @module-rebuild

echo "3. Cleaning up orphaned dependencies (Autoclean)..."
sudo emerge --depclean

echo "4. Safeguarding and cleaning old source files..."
sudo eclean-dist --deep

echo "5. Applying pending configuration updates (dispatch-conf)..."
if [ -e /etc/portage/.autounmask ]; then
    sudo dispatch-conf
fi

echo " Gentoo System Update Complete!"
read -p "Press [Enter] to close..."
