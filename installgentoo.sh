#!/bin/bash

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "Starting Automated Gentoo Setup..."

# 1. Create standard user directories
echo "Creating user directories..."
mkdir -p "$HOME/Documents" "$HOME/Music" "$HOME/Downloads" "$HOME/Pictures" "$HOME/Videos" "$HOME/.config"

# 2. Accept testing
echo "Accepting testing keywords..."
sudo mkdir -p /etc/portage/package.accept_keywords
echo "app-editors/micro ~amd64" | sudo tee -a /etc/portage/package.accept_keywords/editors > /dev/null
echo "x11-misc/slstatus ~amd64" | sudo tee /etc/portage/package.accept_keywords/slstatus

# 3. Install official Gentoo packages
echo "Installing official Gentoo packages..."
sudo emerge --ask=n --noreplace --binpkg-respect-use=y \
    x11-base/xorg-server x11-apps/xinit x11-apps/xrandr x11-libs/libXinerama x11-libs/libXft \
    media-fonts/jetbrains-mono media-fonts/noto-emoji media-fonts/symbols-nerd-font \
    media-fonts/dejavu media-fonts/fontawesome media-fonts/noto media-fonts/noto-cjk \
    x11-misc/rofi media-gfx/feh xfce-base/thunar xfce-base/tumbler sys-fs/udisks \
    x11-themes/adwaita-icon-theme media-gfx/imv media-video/mpv media-sound/pavucontrol \
    x11-misc/dunst x11-misc/clipmenu x11-misc/xsel x11-misc/xclip gnome-extra/polkit-gnome \
    media-sound/playerctl media-sound/cava sys-process/btop \
    app-arch/zip app-arch/unzip app-editors/micro app-editors/nano \
    media-gfx/maim sys-power/power-profiles-daemon \
    x11-misc/lightdm x11-misc/lightdm-gtk-greeter www-client/firefox-bin \
    media-video/pipewire media-video/wireplumber \
    sys-apps/xdg-desktop-portal sys-apps/xdg-desktop-portal-gtk \
    x11-terms/alacritty net-misc/curl x11-apps/xsetroot \
    net-wireless/wireless-tools app-editors/vim

# 4. Copy configuration files to ~/.config
echo "Copying config files to $HOME/.config/..."
if [ -d "$SCRIPT_DIR/config" ]; then
    cp -r "$SCRIPT_DIR/config/"* "$HOME/.config/"
else
    echo "Warning: No config folder found in repository!"
fi

# 5. Compile and install DWM
echo "Copying and compiling DWM in $HOME..."
if [ -d "$SCRIPT_DIR/dwm" ]; then
    rm -rf "$HOME/dwm"
    cp -r "$SCRIPT_DIR/dwm" "$HOME/"
    cd "$HOME/dwm"
    sudo make clean install
    sudo chown -R "$USER:$USER" "$HOME/dwm"
else
    echo "Error: dwm directory not found in repository!"
fi
cd "$SCRIPT_DIR"

# 6. Compile and install custom slock
echo "Copying and compiling slock in $HOME..."
if [ -d "$SCRIPT_DIR/slock" ]; then
    rm -rf "$HOME/slock"
    cp -r "$SCRIPT_DIR/slock" "$HOME/"
    cd "$HOME/slock"
    sudo make clean install
    sudo chmod u+s /usr/local/bin/slock
    sudo chown -R "$USER:$USER" "$HOME/slock"
else
    echo "Error: slock directory not found in repository!"
fi
cd "$SCRIPT_DIR"

# 7. Compile and install custom slstatus
echo "Copying and compiling slstatus in $HOME..."
if [ -d "$SCRIPT_DIR/slstatus" ]; then
    rm -rf "$HOME/slstatus"
    cp -r "$SCRIPT_DIR/slstatus" "$HOME/"
    cd "$HOME/slstatus"
    sudo make clean install
    sudo chown -R "$USER:$USER" "$HOME/slstatus"
else
    echo "Error: slstatus directory not found in repository!"
fi
cd "$SCRIPT_DIR"

# 8. Disable mouse acceleration globally for X11
echo "Disabling mouse acceleration globally..."
sudo mkdir -p /etc/X11/xorg.conf.d
cat << 'EOF' | sudo tee /etc/X11/xorg.conf.d/50-mouse-acceleration.conf > /dev/null
Section "InputClass"
    Identifier "My Mouse"
    MatchIsPointer "yes"
    Option "AccelProfile" "flat"
    Option "AccelSpeed" "0"
EndSection
EOF

# 9. Set up ~/.xprofile
echo "Setting up X11 startup script (.xprofile)..."
cat << 'EOF' > "$HOME/.xprofile"
#!/bin/sh

export XCURSOR_SIZE=24
export XCURSOR_THEME="Adwaita"
export CM_LAUNCHER=rofi
export CM_SELECTIONS="clipboard"

if [ -f "$HOME/.Xresources" ]; then
    xrdb -merge "$HOME/.Xresources"
fi

# Background daemons and wallpaper
feh --bg-scale "$HOME/Pictures/main.png" &
thunar --daemon &
dunst &
slstatus &
clipmenud &
$HOME/.config/scripts/screen.sh &
# GNOME Policykit agent for authentication dialogs
/usr/libexec/polkit-gnome-authentication-agent-1 &

EOF
chmod +x "$HOME/.xprofile"

# 10. Set up ~/.xinitrc 
echo "Setting up X11 startup script (.xinitrc)..."
cat << 'EOF' > "$HOME/.xinitrc"
#!/bin/sh

if [ -f "$HOME/.xprofile" ]; then
    . "$HOME/.xprofile"
fi

exec dbus-run-session dwm
EOF
chmod +x "$HOME/.xinitrc"

# 11. Create desktop session entry for LightDM to recognize DWM
echo "Creating DWM desktop session for LightDM..."
sudo mkdir -p /usr/share/xsessions
cat << EOF | sudo tee /usr/share/xsessions/dwm.desktop > /dev/null
[Desktop Entry]
Name=DWM
Comment=Dynamic Window Manager
Exec=dbus-run-session dwm
Type=Application
X-LightDM-DesktopName=dwm
DesktopNames=dwm
EOF

# 12. Copy wallpaper if present
if [ -f "$SCRIPT_DIR/main.png" ]; then
    echo "Copying wallpaper to $HOME/Pictures/main.png..."
    cp "$SCRIPT_DIR/main.png" "$HOME/Pictures/main.png"
fi

# 13. Ensure local user scripts are executable
if [ -d "$HOME/.config/scripts" ]; then
    chmod +x "$HOME/.config/scripts/"*
fi

# 14. Enable system daemons via Systemd
echo "Enabling system daemons..."
sudo systemctl enable power-profiles-daemon.service
sudo systemctl enable NetworkManager.service
sudo systemctl enable systemd-timesyncd.service
sudo systemctl enable lightdm.service
sudo systemctl enable fstrim.timer

# 15. Enable user-level audio services for Pipewire and Wireplumber
echo "Enabling user audio services..."
systemctl --user enable pipewire.socket
systemctl --user enable pipewire-pulse.socket
systemctl --user enable wireplumber.service

echo "Installation Complete! Reboot your system to enter LightDM."
