#!/bin/bash

set -e

echo "Starting Gentoo System Cleanup..."

if command -v eclean-dist &> /dev/null; then
    sudo eclean-dist --deep
else
    echo "eclean-dist not found."
fi

if [ -n "$(emerge --depclean -pq)" ]; then
    sudo emerge --depclean
else
    echo "No orphaned packages found."
fi

if [ -d "$HOME/.cache/thumbnails" ]; then
    rm -rf "$HOME/.cache/thumbnails/*"
    echo "Thumbnail cache cleared."
else
    echo "No thumbnail cache found."
fi

find "$HOME/.cache" -type f -atime +30 -delete 2>/dev/null || true

echo "Gentoo Cleanup Complete!"

read -p "Press [Enter] to close..."
