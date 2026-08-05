#!/bin/bash

set -e

echo "Configuring make.conf for NVIDIA graphics hardware acceleration..."
if ! grep -q 'VIDEO_CARDS=' /etc/portage/make.conf; then
    echo 'VIDEO_CARDS="nvidia"' >> /etc/portage/make.conf
else
    sudo sed -i 's/VIDEO_CARDS=.*/VIDEO_CARDS="nvidia"/' /etc/portage/make.conf
fi

echo "Setting up package configurations..."
mkdir -p /etc/portage/package.accept_keywords
echo ">=x11-drivers/nvidia-drivers-595.0.0 ~amd64" > /etc/portage/package.accept_keywords/nvidia

mkdir -p /etc/portage/package.use
echo "x11-drivers/nvidia-drivers dist-kernel" >> /etc/portage/package.use/nvidia

echo "Compiling and installing proprietary NVIDIA driver stack..."
emerge --ask x11-drivers/nvidia-drivers

echo "Syncing module bindings across the active distribution kernel target images..."
emerge @module-rebuild

echo "Activating persistent management service background daemon..."
systemctl enable nvidia-persistenced.service

echo "NVIDIA configuration completed successfully! Changes will take effect upon system reboot."
