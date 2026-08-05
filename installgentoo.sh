#!/bin/bash

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE}")" && pwd)"

echo "Starting minimal automated Gentoo installation..."

echo "Creating user directories..."
mkdir -p "$HOME/Documents" "$HOME/Music" "$HOME/Downloads" "$HOME/Pictures" "$HOME/Videos" "$HOME/.config"

echo "Accepting testing keyword for micro text editor..."
sudo mkdir -p /etc/portage/package.accept_keywords
echo "app-editors/micro ~amd64" | sudo tee -a /etc/portage/package.accept_keywords/editors > /dev/null

echo "Updating Portage world requirements..."
sudo emerge --ask=n --noreplace \
    x11-base/xorg-server x11-apps/xinit x11-apps/xrandr x11-libs/libXinerama x11-libs/libXft \
    media-fonts/liberation-fonts media-fonts/symbols-nerd-font x11-misc/rofi media-gfx/feh \
    xfce-base/thunar xfce-base/tumbler sys-fs/udisks x11-themes/adwaita-icon-theme \
    media-gfx/imv media-video/mpv media-sound/pavucontrol x11-misc/dunst \
    x11-misc/clipmenu x11-misc/xsel x11-misc/xclip lxqt-base/lxqt-policykit \
    media-sound/playerctl sys-power/acpilight media-sound/cava sys-process/btop \
    app-arch/zip app-arch/unzip app-editors/micro app-editors/nano \
    media-gfx/maim x11-misc/picom sys-power/power-profiles-daemon

echo "Copying config files to $HOME/.config/..."
if [ -d "$SCRIPT_DIR/config" ]; then
    cp -r "$SCRIPT_DIR/config/"* "$HOME/.config/"
else
    echo "Warning: No config folder found in repository!"
fi

echo "Copying and compiling DWM & ST in $HOME..."
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

if [ -d "$SCRIPT_DIR/st" ]; then
    rm -rf "$HOME/st"
    cp -r "$SCRIPT_DIR/st" "$HOME/"
    cd "$HOME/st"
    sudo make clean install
    sudo chown -R "$USER:$USER" "$HOME/st"
else
    echo "Error: st directory not found in repository!"
fi

cd "$SCRIPT_DIR"

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

echo "Setting up X11 startup script (~/.xinitrc)..."
cat << 'EOF' > "$HOME/.xinitrc"
#!/bin/sh

if [ -z "$DBUS_SESSION_BUS_ADDRESS" ]; then
    eval $(dbus-launch --sh-syntax --exit-with-session)
fi

feh --bg-scale "$HOME/Pictures/main.png" &
picom &

thunar --daemon &
dunst &
clipmenud &

while true; do
    xsetroot -name "$(date '+%Y-%m-%d %H:%M')"
    sleep 60
done &

/usr/libexec/lxqt-policykit-agent &

exec dbus-run-session dwm
EOF

chmod +x "$HOME/.xinitrc"

if [ -f "$SCRIPT_DIR/main.png" ]; then
    echo "Copying wallpaper to $HOME/Pictures/main.png..."
    cp "$SCRIPT_DIR/main.png" "$HOME/Pictures/main.png"
fi

if [ -d "$HOME/.config/scripts" ]; then
    chmod +x "$HOME/.config/scripts/"*
fi

echo "Fixing home directory paths for $USER..."
find "$HOME/.config" -type f -exec sed -i "s|/home/[^/]*|$HOME|g" {} + 2>/dev/null || true

echo "Enabling background system daemons..."
sudo systemctl enable power-profiles-daemon.service
systemctl enable NetworkManager
systemctl enable systemd-timesyncd
echo "Installation Complete! Type 'startx' to enter your clean DWM environment."
