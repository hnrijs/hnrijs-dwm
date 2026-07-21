#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

# Get the directory where this script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "Starting minimal automated installation..."

# 0. Ensure git is installed first
if ! command -v git &> /dev/null; then
    echo "Installing git..."
    sudo pacman -Sy --noconfirm git
fi

# 1. Create standard home directories
echo "Creating user directories..."
mkdir -p "$HOME/Documents" "$HOME/Music" "$HOME/Downloads" "$HOME/Pictures" "$HOME/Videos" "$HOME/.config"

# 2. Update system and install official pacman packages
# (Only essential fonts: ttf-jetbrains-mono-nerd and noto-fonts-emoji)
echo "Installing official pacman packages..."
sudo pacman -S --needed --noconfirm \
    base-devel wget xorg-server xorg-xinit libx11 libxft libxinerama \
    feh thunar rofi imv cava btop playerctl alacritty zip unzip polkit-gnome \
    xclip maim power-profiles-daemon ttf-jetbrains-mono-nerd noto-fonts-emoji \
    gtk3 fastfetch pavucontrol nwg-look mpv brightnessctl xsettingsd micro \
    networkmanager network-manager-applet lightdm lightdm-gtk-greeter

# 3. Check and install yay AUR helper
if ! command -v yay &> /dev/null; then
    echo "Installing yay AUR helper..."
    mkdir -p /tmp/yay-build
    git clone https://aur.archlinux.org/yay.git /tmp/yay-build/yay
    cd /tmp/yay-build/yay
    makepkg -si --noconfirm
    cd -
    rm -rf /tmp/yay-build
fi

# 4. Install AUR packages
echo "Installing AUR packages..."
yay -S --noconfirm helium-browser-bin rofi-greenclip

# 5. Copy configuration files (.config directory)
echo "Copying config files to $HOME/.config/..."
if [ -d "$SCRIPT_DIR/config" ]; then
    cp -r "$SCRIPT_DIR/config/"* "$HOME/.config/"
else
    echo "Warning: No config folder found in repository!"
fi

# 6. Copy DWM and ST to $HOME, compile them, and fix ownership permissions
echo "Copying and compiling DWM & ST in $HOME..."

if [ -d "$SCRIPT_DIR/dwm" ]; then
    cp -r "$SCRIPT_DIR/dwm" "$HOME/"
    cd "$HOME/dwm"
    sudo make clean install
    sudo chown -R "$USER:$USER" "$HOME/dwm"
else
    echo "Error: dwm directory not found in repo!"
fi

if [ -d "$SCRIPT_DIR/st" ]; then
    cp -r "$SCRIPT_DIR/st" "$HOME/"
    cd "$HOME/st"
    sudo make clean install
    sudo chown -R "$USER:$USER" "$HOME/st"
else
    echo "Error: st directory not found in repo!"
fi

cd "$SCRIPT_DIR"

# 7. Setup DWM startup files (.xinitrc and .xsession)
echo "Setting up X11 startup scripts..."
cat << 'EOF' > "$HOME/.xinitrc"
#!/bin/bash

# Autostart background processes
dex --autostart --environment dwm &
feh --bg-scale "$HOME/Pictures/main.png" &
nm-applet &
/usr/lib/polkit-gnome/polkit-gnome-authentication-agent-1 &

# Update DWM status bar using your script
while true; do
    if [ -f "$HOME/.config/scripts/status_power.sh" ]; then
        xsetroot -name "$($HOME/.config/scripts/status_power.sh)"
    fi
    sleep 1
done &

# Launch DWM
exec dwm
EOF

cp "$HOME/.xinitrc" "$HOME/.xsession"
chmod +x "$HOME/.xinitrc" "$HOME/.xsession"

# 8. Create xsessions entry for LightDM
echo "Creating DWM desktop session for LightDM..."
sudo mkdir -p /usr/share/xsessions
cat << 'EOF' | sudo tee /usr/share/xsessions/dwm.desktop > /dev/null
[Desktop Entry]
Name=dwm
Comment=Dynamic Window Manager
Exec=dwm
Type=Application
X-LightDM-DesktopName=dwm
DesktopNames=dwm
EOF

# 9. Copy wallpaper to Pictures directory
if [ -f "$SCRIPT_DIR/main.png" ]; then
    echo "Copying wallpaper to $HOME/Pictures/main.png..."
    cp "$SCRIPT_DIR/main.png" "$HOME/Pictures/main.png"
fi

# 10. Make custom scripts executable
if [ -d "$HOME/.config/scripts" ]; then
    chmod +x "$HOME/.config/scripts/"*
fi

# 11. Dynamically fix home paths in configs for current user
echo "Fixing home directory paths for $USER..."
find "$HOME/.config" -type f -exec sed -i "s|/home/[^/]*|$HOME|g" {} + 2>/dev/null || true

# 12. Enable system and user services
echo "Enabling services..."
systemctl --user enable --now greenclip.service || true
sudo systemctl enable --now power-profiles-daemon
sudo systemctl enable --now NetworkManager
sudo systemctl enable lightdm


echo " Installation Complete! Rebooting in 5 seconds..."

sleep 5
sudo reboot
