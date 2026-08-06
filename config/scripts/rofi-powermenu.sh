#!/usr/bin/env bash

lastlogin="$(last $USER | head -n1 | tr -s ' ' | cut -d' ' -f5,6,7)"
uptime="$(uptime -p | sed -e 's/up //g;s/ minutes/m/g;s/ hours*,/h/g')"

hibernate=''
shutdown='󰐥'
reboot=''
lock='󰌾'
suspend='󰤄'
logout='󰍃'

rofi_cmd() {
	rofi -dmenu \
		-p "  $USER" \
		-mesg "  Uptime: $uptime" \
		-theme-str "imagebox { background-image: url(\"$HOME/Pictures/main.png\", height); }" \
		-config "$HOME/.config/rofi/rofi-powermenu-config.rasi"
}

run_rofi() {
	echo -e "$lock\n$reboot\n$logout\n$suspend\n$shutdown\n$hibernate" | rofi_cmd
}

run_cmd() {
	if [[ $1 == '--shutdown' ]]; then
		systemctl poweroff || loginctl poweroff
	elif [[ $1 == '--reboot' ]]; then
		systemctl reboot || loginctl reboot
	elif [[ $1 == '--hibernate' ]]; then
		systemctl hibernate
	elif [[ $1 == '--suspend' ]]; then
		amixer set Master mute
		systemctl suspend
	elif [[ $1 == '--logout' ]]; then
		pkill dwm  
	fi
}

chosen="$(run_rofi)"
case ${chosen} in
    $shutdown)
		run_cmd --shutdown
        ;;
    $reboot)
		run_cmd --reboot
        ;;
    $hibernate)
		run_cmd --hibernate
        ;;
    $lock)
        slock
		;;
    $suspend)
        slock & sleep 0.5 
		run_cmd --suspend
        ;;
    $logout)
		run_cmd --logout
        ;;
esac
