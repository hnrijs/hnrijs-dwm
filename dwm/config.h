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
    /*               fg         bg          border   */
    [SchemeNorm] = { col_fg,    col_bg,     "#333333" },
    [SchemeSel]  = { col_fg_active, col_bg, "#FFFFFF" },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
    /* xprop(1):
     *    WM_CLASS(STRING) = instance, class
     *    WM_NAME(STRING) = title
     */
    /* class      instance    title       tags mask     isfloating   monitor */
    { "Gimp",     NULL,       NULL,       0,            1,           -1 },
    { "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
    /* symbol     arrange function */
    { "[]=",      tile },    /* first entry is default */
    { "><>",      NULL },    /* no layout function means floating behavior */
    { "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
    { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
    { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
    { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]       = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_bg, "-nf", col_fg, "-sb", col_bg, "-sf", col_fg_active, NULL };
static const char *termcmd[]        = { "alacritty", NULL };
static const char *roficmd[]        = { "rofi", "-show", "run", NULL };
static const char *thunarcmd[]      = { "thunar", NULL };
static const char *browsercmd[]     = { "firefox-bin", NULL };
static const char *comcmd[]         = { "signal-desktop", NULL };
static const char *codecmd[]        = { "vscode", NULL };
static const char *pavucmd[]        = { "pavucontrol", NULL };
static const char *nmtuicmd[]       = { "alacritty", "--class", "nmtui", "-e", "nmtui", NULL };
static const char *lockcmd[]        = { "slock", NULL };
static const char *powermenu[]      = { "sh", "-c", "$HOME/.config/scripts/rofi-powermenu.sh", NULL };
static const char *powerprof[]      = { "sh", "-c", "$HOME/.config/scripts/power_profile.sh", NULL };
static const char *screenshot[]     = { "sh", "-c", "maim -s | xclip -selection clipboard -t image/png", NULL };
static const char *screenall[]      = { "sh", "-c", "mkdir -p ~/Pictures/Screenshots && f=~/Pictures/Screenshots/scr_$(date +%s).png && maim \"$f\" && xclip -selection clipboard -t image/png -i \"$f\"", NULL };
static const char *screensrc[]      = { "sh", "-c", "$HOME/.config/scripts/screen_search.sh", NULL };

static const Key keys[] = {
    /* modifier                     key        function        argument */
    { MODKEY,                       XK_Return,          spawn,          {.v = termcmd } },
    { MODKEY,                       XK_space,           spawn,          {.v = roficmd } },
    { MODKEY,                       XK_f,               spawn,          {.v = thunarcmd } },
    { MODKEY,                       XK_b,               spawn,          {.v = browsercmd } },
    { MODKEY,                       XK_s,               spawn,          {.v = comcmd } },
    { MODKEY,                       XK_c,               spawn,          {.v = codecmd } },
    { MODKEY|ShiftMask,             XK_a,               spawn,          {.v = pavucmd } },
    { MODKEY|ShiftMask,             XK_n,               spawn,          {.v = nmtuicmd } },

    /* Scripts & Power */
    { MODKEY|ShiftMask,             XK_L,               spawn,          {.v = lockcmd } },
    { MODKEY,                       XK_Escape,          spawn,          {.v = powermenu } },
    { MODKEY|ShiftMask,             XK_p,               spawn,          {.v = powerprof } },

    /* Utilities */
    { MODKEY|ShiftMask,             XK_s,               spawn,          {.v = screenshot } },
    { MODKEY|ShiftMask,             XK_x,               spawn,          {.v = screenall } },
    { MODKEY|ShiftMask,             XK_e,               spawn,          {.v = screensrc } },

    { MODKEY|ShiftMask,             XK_b,      togglebar,      {0} },
    { MODKEY,                       XK_Right,  focusstack,     {.i = +1 } },
    { MODKEY,                       XK_Left,   focusstack,     {.i = -1 } },
    { MODKEY,                       XK_d,      incnmaster,     {.i = +1 } },
    { MODKEY,                       XK_a,      incnmaster,     {.i = -1 } },
    { MODKEY|ShiftMask,             XK_Left,   setmfact,       {.f = -0.05} },
    { MODKEY|ShiftMask,             XK_Right,  setmfact,       {.f = +0.05} },
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
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
    /* click                event mask      button          function        argument */
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
