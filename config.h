/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static char font[]            = "monospace:size=10";
static char dmenufont[]       = "monospace:size=10";
static const char *fonts[]          = { font };
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

#define TERM "st"
#define TERMCLASS "St"
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class,                    instance,        title,          tags,   isfloating, isterminal, noswallow, monitor */
	{ "Gimp",                    NULL,            NULL,           1 << 8, 0,          0,          0,         -1 },
	{ TERMCLASS,                 NULL,            NULL,           0,      0,          1,          0,         -1 },
        { "Desktop-session-exit.py", NULL,            NULL,           0,      1,          0,          0,         -1 },
	{ NULL,                      NULL,            "Event Tester", 0,      0,          0,          1,         -1 },
        { "Brave-browser",           "brave-browser", NULL,           9 << 8, 0,          0,          0,         -1 },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
        /* symbol     arrange function */
        { "[]=",      tile },    /* first entry is default */
        { "[M]",      monocle },
        { "[@]",      spiral },
        { "[\\]",     dwindle },
        { "H[]",      deck },
        { "TTT",      bstack },
        { "===",      bstackhoriz },
        { "HHH",      grid },
        { "###",      nrowgrid },
        { "---",      horizgrid },
        { ":::",      gaplessgrid },
        { "|M|",      centeredmaster },
        { ">M>",      centeredfloatingmaster },
        { "><>",      NULL },    /* no layout function means floating behavior */
        { NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
        { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
        { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
        { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
        { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
        { MOD,  XK_j,   ACTION##stack,  {.i = INC(+1) } }, \
        { MOD,  XK_k,   ACTION##stack,  {.i = INC(-1) } }, \
        { MOD,  XK_v,   ACTION##stack,  {.i = 0 } }

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { TERM, NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { "st", "-t", scratchpadname, "-g", "120x34", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "color0",		STRING,	&normbordercolor },
		{ "color8",		STRING,	&selbordercolor },
		{ "color0",		STRING,	&normbgcolor },
		{ "color4",		STRING,	&normfgcolor },
		{ "color0",		STRING,	&selfgcolor },
		{ "color4",		STRING,	&selbgcolor },
		{ "borderpx",		INTEGER, &borderpx },
		{ "snap",		INTEGER, &snap },
		{ "showbar",		INTEGER, &showbar },
		{ "topbar",		INTEGER, &topbar },
		{ "nmaster",		INTEGER, &nmaster },
		{ "resizehints",	INTEGER, &resizehints },
		{ "mfact",		FLOAT,	&mfact },
		{ "gappih",		INTEGER, &gappih },
		{ "gappiv",		INTEGER, &gappiv },
		{ "gappoh",		INTEGER, &gappoh },
		{ "gappov",		INTEGER, &gappov },
		{ "swallowfloating",	INTEGER, &swallowfloating },
		{ "smartgaps",		INTEGER, &smartgaps }
};

static Key keys[] = {
        /* modifier                     key        function        argument */
        STACKKEYS(MODKEY,                          focus),
        STACKKEYS(MODKEY|ShiftMask,                push),
        { MODKEY,                       XK_r,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,		XK_r,	   livereload_xresources,	   {.v = NULL } },
        { MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
        { MODKEY|ShiftMask,             XK_minus,  togglescratch,  {.v = scratchpadcmd } },
        { MODKEY,                       XK_b,      togglebar,      {0} },
        { MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
        { MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
        { Mod1Mask|ShiftMask,           XK_h,      setmfact,       {.f = -0.05} },
        { Mod1Mask|ShiftMask,           XK_l,      setmfact,       {.f = +0.05} },
        { Mod1Mask|ShiftMask,           XK_equal,  setmfact,       {.f = mfact+1} },
        { MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
        { MODKEY|Mod1Mask,              XK_u,      incrgaps,       {.i = +1 } },
        { MODKEY|Mod1Mask|ShiftMask,    XK_u,      incrgaps,       {.i = -1 } },
        { MODKEY|Mod1Mask,              XK_i,      incrigaps,      {.i = +1 } },
        { MODKEY|Mod1Mask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } },
        { MODKEY|Mod1Mask,              XK_o,      incrogaps,      {.i = +1 } },
        { MODKEY|Mod1Mask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } },
        { MODKEY|Mod1Mask,              XK_6,      incrihgaps,     {.i = +1 } },
        { MODKEY|Mod1Mask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } },
        { MODKEY|Mod1Mask,              XK_7,      incrivgaps,     {.i = +1 } },
        { MODKEY|Mod1Mask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } },
        { MODKEY|Mod1Mask,              XK_8,      incrohgaps,     {.i = +1 } },
        { MODKEY|Mod1Mask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } },
        { MODKEY|Mod1Mask,              XK_9,      incrovgaps,     {.i = +1 } },
        { MODKEY|Mod1Mask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } },
        { MODKEY|Mod1Mask,              XK_0,      togglegaps,     {0} },
        { MODKEY|Mod1Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
        { MODKEY,                       XK_Tab,    view,           {0} },
        { MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
        { MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} }, // tile
        { MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} }, // monocle
        { MODKEY|ShiftMask,             XK_f,      togglefullscr,  {0} },
        { MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} }, // spiral
        { MODKEY,                       XK_s,      setlayout,      {.v = &layouts[5]} }, // bstack
        { MODKEY,                       XK_space,  setlayout,      {0} },
        { MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
        { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
        { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
        { MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
        { MODKEY,                       XK_period, focusmon,       {.i = +1 } },
        { MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
        { MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
        { Mod1Mask|ShiftMask,           XK_q,      quit,           {0} },
        TAGKEYS(                        XK_1,                      0)
        TAGKEYS(                        XK_2,                      1)
        TAGKEYS(                        XK_3,                      2)
        TAGKEYS(                        XK_4,                      3)
        TAGKEYS(                        XK_5,                      4)
        TAGKEYS(                        XK_6,                      5)
        TAGKEYS(                        XK_7,                      6)
        TAGKEYS(                        XK_8,                      7)
        TAGKEYS(                        XK_9,                      8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
        /* click                event mask      button          function        argument */
        { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
        { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
        { ClkWinTitle,          0,              Button2,        zoom,           {0} },
        { ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1 } },
        { ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2 } },
        { ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3 } },
        { ClkStatusText,        0,              Button4,        sigstatusbar,   {.i = 4 } },
        { ClkStatusText,        0,              Button5,        sigstatusbar,   {.i = 5 } },
        { ClkStatusText,        ShiftMask,      Button1,        sigstatusbar,   {.i = 6} },
        { ClkStatusText,        ShiftMask,      Button3,        spawn,          SHCMD(TERM " -e vim ~/repos/dwmblocks/blocks.h") },
        { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
        { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
        { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
        { ClkTagBar,            0,              Button1,        view,           {0} },
        { ClkTagBar,            0,              Button3,        toggleview,     {0} },
        { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
        { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

