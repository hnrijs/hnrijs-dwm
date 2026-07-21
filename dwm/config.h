/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;
static const unsigned int snap      = 32;
static const int showbar            = 1;
static const int topbar             = 1;
static const unsigned int refreshrate = 120;
static const char *fonts[]          = { "JetBrainsMono Nerd Font:size=10" };
static const char dmenufont[]       = "JetBrainsMono Nerd Font:size=10";

static const char col_bg[]          = "#000000";
static const char col_fg[]          = "#888888";
static const char col_fg_active[]   = "#FFFFFF";

static const char *colors[][3]      = {
	/*                   fg         bg          border   */
	[SchemeNorm] = { col_fg,    col_bg,     "#333333" },
	[SchemeSel]  = { col_fg_active, col_bg, "#FFFFFF" },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" };

static const Rule rules[] = {
	/* class     instance    title       tags mask     isfloating   monitor */
	{ "Pavucontrol", NULL,    NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55;
static const int nmaster     = 1;
static const int resizehints = 1;
static const int lockfullscreen = 1;

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },
	{ "><>",      NULL },
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#include <X11/XF86keysym.h>

static char dmenumon[2] = "0"; 
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_bg, "-nf", col_fg, "-sb", col_fg_active, "-sf", col_bg, NULL };

static const char *termcmd[]        = { "alacritty", NULL };
static const char *roficmd[]        = { "rofi", "-show", "run", NULL };
static const char *thunarcmd[]      = { "thunar", NULL };
static const char *browsercmd[]     = { "helium-browser", NULL };
static const char *pavucmd[]        = { "pavucontrol", NULL };
static const char *nmtuicmd[]       = { "alacritty", "--class", "nmtui", "-e", "nmtui", NULL };

static const char *lockcmd[]        = { "/home/h/.config/scripts/i3exit.sh", "lock", NULL };
static const char *powermenu[]      = { "/home/h/.config/scripts/rofi-powermenu.sh", NULL };
static const char *powerprof[]      = { "/home/h/.config/scripts/power_profile.sh", NULL };
static const char *sysupdate[]      = { "alacritty", "-e", "/home/h/.config/scripts/system_update.sh", NULL };
static const char *sysclean[]       = { "alacritty", "-e", "/home/h/.config/scripts/system_clean.sh", NULL };

static const char *screenshot[]     = { "sh", "-c", "maim -s | xclip -selection clipboard -t image/png", NULL };
static const char *clipmenu[]       = { "sh", "-c", "rofi -modi \"clipboard:greenclip print\" -show clipboard -run-command \"{cmd}\"", NULL };
static const char *clipclear[]      = { "greenclip", "clear", NULL };

static const char *volup[]          = { "sh", "-c", "pactl set-sink-volume @DEFAULT_SINK@ +5%", NULL };
static const char *voldown[]        = { "sh", "-c", "pactl set-sink-volume @DEFAULT_SINK@ -5%", NULL };
static const char *volmute[]        = { "sh", "-c", "pactl set-sink-mute @DEFAULT_SINK@ toggle", NULL };
static const char *micmute[]        = { "sh", "-c", "pactl set-source-mute @DEFAULT_SOURCE@ toggle", NULL };
static const char *brightup[]       = { "brightnessctl", "set", "+5%", NULL };
static const char *brightdown[]     = { "brightnessctl", "set", "5%-", NULL };
static const char *mediaplay[]      = { "playerctl", "play-pause", NULL };

static const Key keys[] = {
	/* Applications */
	{ MODKEY,                       XK_Return,          spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_space,           spawn,          {.v = roficmd } },
	{ MODKEY,                       XK_f,               spawn,          {.v = thunarcmd } },
	{ MODKEY,                       XK_b,               spawn,          {.v = browsercmd } },
	{ MODKEY|ShiftMask,             XK_a,               spawn,          {.v = pavucmd } },
	{ MODKEY|ShiftMask,             XK_n,               spawn,          {.v = nmtuicmd } },

	/* Scripts & Power */
	{ MODKEY|ShiftMask,             XK_L,               spawn,          {.v = lockcmd } },
	{ MODKEY,                       XK_Escape,          spawn,          {.v = powermenu } },
	{ MODKEY|ShiftMask,             XK_p,               spawn,          {.v = powerprof } },
	{ MODKEY|ShiftMask,             XK_u,               spawn,          {.v = sysupdate } },
	{ MODKEY|ShiftMask,             XK_c,               spawn,          {.v = sysclean } },

	/* Utilities */
	{ MODKEY|ShiftMask,             XK_s,               spawn,          {.v = screenshot } },
	{ MODKEY,                       XK_v,               spawn,          {.v = clipmenu } },
	{ MODKEY|ShiftMask,             XK_v,               spawn,          {.v = clipclear } },

	/* Hardware Keys */
	{ 0,                            XF86XK_AudioRaiseVolume, spawn,     {.v = volup } },
	{ 0,                            XF86XK_AudioLowerVolume, spawn,     {.v = voldown } },
	{ 0,                            XF86XK_AudioMute,        spawn,     {.v = volmute } },
	{ 0,                            XF86XK_AudioMicMute,     spawn,     {.v = micmute } },
	{ 0,                            XF86XK_MonBrightnessUp,  spawn,     {.v = brightup } },
	{ 0,                            XF86XK_MonBrightnessDown, spawn,    {.v = brightdown } },
	{ 0,                            XF86XK_AudioPlay,        spawn,     {.v = mediaplay } },

	/* Layout & Window Controls */
	{ MODKEY,                       XK_q,               killclient,     {0} },
	{ MODKEY,                       XK_Up,              focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_Down,            focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_w,               setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_s,               setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_z,               togglefloating, {0} },
	{ MODKEY,                       XK_e,               zoom,           {0} },
	
	/* Master/Stack & Resizing Controls */
	{ MODKEY,                       XK_Left,            setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_Right,           setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_b,               togglebar,      {0} },

	/* Quit DWM */
	{ MODKEY|ShiftMask,             XK_e,               quit,           {0} },

	/* Workspaces (1-10) */
	TAGKEYS(                        XK_1,                               0)
	TAGKEYS(                        XK_2,                               1)
	TAGKEYS(                        XK_3,                               2)
	TAGKEYS(                        XK_4,                               3)
	TAGKEYS(                        XK_5,                               4)
	TAGKEYS(                        XK_6,                               5)
	TAGKEYS(                        XK_7,                               6)
	TAGKEYS(                        XK_8,                               7)
	TAGKEYS(                        XK_9,                               8)
	TAGKEYS(                        XK_0,                               9)
};

/* button definitions */
static const Button buttons[] = {
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
};
