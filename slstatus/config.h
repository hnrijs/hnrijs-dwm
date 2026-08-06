/* interval between updates (in ms) */
const unsigned int interval = 1000;

/* text to show if no value can be retrieved */
static const char unknown_str[] = "n/a";

/* maximum output string length */
#define MAXLEN 2048

static const struct arg args[] = {
    /* function     format               argument */
    { disk_perc,    "FS %s%% | ",        "/" },
    { ram_perc,     "RAM %s%% | ",       NULL },
    { cpu_perc,     "CPU %s%% | ",       NULL },
    { run_command,  "PWR %s | ",         "powerprofilesctl get 2>/dev/null | sed 's/power-saver/S/;s/balanced/B/;s/performance/P/'" },
    { run_command,  "VPN %s | ",         "ip link show up 2>/dev/null | grep -qE 'tun[0-9]+|wg[0-9]+|proton|pvpn' && echo 'ON' || echo 'OFF'" },
    { run_command,  "VOL %s | ",         "pactl get-sink-volume @DEFAULT_SINK@ 2>/dev/null | grep -o '[0-9]*%' | head -n 1" },
    { battery_perc, "BAT %s%% | ",       "BAT1" },
    { keymap,       "KBD %s | ",         NULL },
    { datetime,     "%s ",               "%Y-%m-%d %H:%M" },
};
