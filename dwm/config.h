/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 2;
static const unsigned int snap      = 32;
static const int showbar            = 1;
static const int topbar             = 1;
static const unsigned int refreshrate = 120;
static const int swallowfloating    = 0; 
static const unsigned int systraypinning = 0;   
static const unsigned int systrayonleft  = 0;   
static const unsigned int systrayspacing = 2;   
static const int systraypinningfailfirst = 1;   
static const int showsystray             = 1;   
static const char *fonts[]          = { "JetBrainsMono Nerd Font:size=12" };
static const char dmenufont[]       = "JetBrainsMono Nerd Font:size=12";

static const char col_bg[]          = "#000000";
static const char col_fg[]          = "#888888";
static const char col_fg_active[]   = "#FFFFFF";

static const char *colors[][3]      = {
    /*               fg         bg          border   */
    [SchemeNorm] = { col_fg,    col_bg,     "#333333" },
    [SchemeSel]  = { col_fg_active, col_bg, "#FFFFFF" },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
    /* class      instance    title       tags mask     isfloating   monitor */
    { "Gimp",     NULL,       NULL,       0,            1,           -1 },
    { "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55;
static const int nmaster     = 1;
static const int resizehints = 1;
static const int lockfullscreen = 1;

static const Layout layouts[] = {
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

/* commands */
static char dmenumon[2] = "0";
static const char *dmenucmd[]       = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_bg, "-nf", col_fg, "-sb", col_bg, "-sf", col_fg_active, NULL };
static const char *termcmd[]        = { "alacritty", NULL };
static const char *roficmd[]        = { "rofi", "-show", "run", NULL };
static const char *clipmenucmd[]    = { "clipmenu", NULL };
static const char *thunarcmd[]      = { "thunar", NULL };
static const char *browsercmd[]     = { "firefox-bin", NULL };
static const char *comcmd[]         = { "signal-desktop", NULL };
static const char *codecmd[]        = { "vscode", NULL };
static const char *pavucmd[]        = { "pavucontrol", NULL };
static const char *nmtuicmd[]       = { "alacritty", "--class", "nmtui", "-e", "nmtui", NULL };
static const char *lockcmd[]        = { "slock", NULL };
static const char *obscmd[]         = { "obs", NULL };
static const char *resolvecmd[]     = { "resolve", NULL };
static const char *audaciouscmd[]   = { "audacious", NULL };
static const char *protonvpncmd[]   = { "protonvpn-app", NULL };

static const char *powermenu[]      = { "sh", "-c", "$HOME/.config/scripts/rofi-powermenu.sh", NULL };
static const char *powerprof[]      = { "sh", "-c", "$HOME/.config/scripts/power_profile.sh", NULL };
static const char *screenshot[]     = { "sh", "-c", "mkdir -p $HOME/Pictures/Screenshots && f=$HOME/Pictures/Screenshots/scr_$(date +%s).png && maim -s \"$f\" && xclip -selection clipboard -t image/png -i \"$f\"", NULL };
static const char *screenall[]      = { "sh", "-c", "mkdir -p $HOME/Pictures/Screenshots && f=$HOME/Pictures/Screenshots/scr_$(date +%s).png && maim \"$f\" && xclip -selection clipboard -t image/png -i \"$f\"", NULL };
static const char *screensrc[]      = { "sh", "-c", "$HOME/.config/scripts/screen_search.sh", NULL };
static const char *updcmd[]         = { "alacritty", "-e", "bash", "-c", "$HOME/scripts/system_update.sh", NULL };
static const char *cleancmd[]       = { "alacritty", "-e", "bash", "-c", "$HOME/scripts/system_clean.sh", NULL };

/* Audio and Brightness Commands */
static const char *upvol[]   = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "+5%",     NULL };
static const char *downvol[] = { "pactl", "set-sink-volume", "@DEFAULT_SINK@", "-5%",     NULL };
static const char *mutevol[] = { "pactl", "set-sink-mute",   "@DEFAULT_SINK@", "toggle",  NULL };
static const char *brupcmd[] = { "brightnessctl", "set", "5%+", NULL };
static const char *brdowncmd[] = { "brightnessctl", "set", "5%-", NULL };



static const Key keys[] = {
    /* modifier                     key        function        argument */
    { MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
    { MODKEY,                       XK_space,  spawn,          {.v = roficmd } },
    { MODKEY,                       XK_v,      spawn,          {.v = clipmenucmd } },
    { MODKEY,                       XK_f,      spawn,          {.v = thunarcmd } },
    { MODKEY,                       XK_b,      spawn,          {.v = browsercmd } },
    { MODKEY,                       XK_s,      spawn,          {.v = comcmd } },
    { MODKEY,                       XK_c,      spawn,          {.v = codecmd } },
    { MODKEY,                       XK_o,      spawn,          {.v = obscmd } },
    { MODKEY,                       XK_r,      spawn,          {.v = resolvecmd } },
    { MODKEY,                       XK_m,      spawn,          {.v = audaciouscmd } },
    { MODKEY|ShiftMask,             XK_p,      spawn,          {.v = protonvpncmd } },
    { MODKEY|ShiftMask,             XK_a,      spawn,          {.v = pavucmd } },
    { MODKEY|ShiftMask,             XK_n,      spawn,          {.v = nmtuicmd } },
    /* Update & Cleanup */
    { MODKEY|ShiftMask,             XK_u,      spawn,          {.v = updcmd } },
    { MODKEY|ShiftMask,             XK_c,      spawn,          {.v = cleancmd } },

    /* Scripts & Power */
    { MODKEY|ShiftMask,             XK_l,      spawn,          {.v = lockcmd } },
    { MODKEY,                       XK_Escape, spawn,          {.v = powermenu } },
    { MODKEY|ShiftMask,             XK_p,      spawn,          {.v = powerprof } },

    /* Utilities */
    { MODKEY|ShiftMask,             XK_s,      spawn,          {.v = screenshot } },
    { MODKEY|ShiftMask,             XK_x,      spawn,          {.v = screenall } },
    { MODKEY|ShiftMask,             XK_e,      spawn,          {.v = screensrc } },

    /* Media Keys (Volume & Brightness) */
    { 0, XF86XK_AudioRaiseVolume,              spawn,          {.v = upvol } },
    { 0, XF86XK_AudioLowerVolume,              spawn,          {.v = downvol } },
    { 0, XF86XK_AudioMute,                     spawn,          {.v = mutevol } },
    { 0, XF86XK_MonBrightnessUp,               spawn,          {.v = brupcmd } },
    { 0, XF86XK_MonBrightnessDown,             spawn,          {.v = brdowncmd } },

    /* Window Management */
    { MODKEY|ShiftMask,             XK_b,      togglebar,      {0} },
    { MODKEY,                       XK_Right,  focusstack,     {.i = +1 } },
    { MODKEY,                       XK_Left,   focusstack,     {.i = -1 } },
    { MODKEY,                       XK_d,      incnmaster,     {.i = +1 } },
    { MODKEY,                       XK_a,      incnmaster,     {.i = -1 } },
    { MODKEY|ShiftMask,             XK_Left,   setmfact,       {.f = -0.05} },
    { MODKEY|ShiftMask,             XK_Right,  setmfact,       {.f = +0.05} },

	{ MODKEY|ShiftMask,             XK_Up,     setcfact,       {.f = +0.25} },
    { MODKEY|ShiftMask,             XK_Down,   setcfact,       {.f = -0.25} },
    { MODKEY|ShiftMask,             XK_g,      setcfact,       {.f =  0.00} },
    { MODKEY,                       XK_e,      zoom,           {0} },
    { MODKEY,                       XK_Tab,    view,           {0} },
    { MODKEY,                       XK_q,      killclient,     {0} },
    { MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
    { MODKEY|ShiftMask,             XK_z,      setlayout,      {.v = &layouts[1]} },
    { MODKEY,                       XK_w,      setlayout,      {.v = &layouts[2]} },
    { MODKEY,                       XK_z,      togglefloating, {0} },
    { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
    { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
    { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
    { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
    { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
    { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
    TAGKEYS(                        XK_1,                      0)
    TAGKEYS(                        XK_2,                      1)
    TAGKEYS(                        XK_3,                      2)
    TAGKEYS(                        XK_4,                      3)
    TAGKEYS(                        XK_5,                      4)
    TAGKEYS(                        XK_6,                      5)
    TAGKEYS(                        XK_7,                      6)
    TAGKEYS(                        XK_8,                      7)
    TAGKEYS(                        XK_9,                      8)
    { MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
static const Button buttons[] = {
    { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
    { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
    { ClkWinTitle,          0,              Button2,        zoom,           {0} },
    { ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
    { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
    { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
