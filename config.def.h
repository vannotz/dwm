/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
#define TERM "alacritty"
#define BROWSER "librewolf"

/* appearance */
static const unsigned int borderpx		= 1;	/* border pixel of windows */
static const unsigned int snap			= 32;	/* snap pixel */
static const unsigned int systraypinning	= 0;	/* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft		= 0;	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing	= 2;	/* systray spacing */
static const int systraypinningfailfirst	= 1;	/* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray			= 1;	/* 0 means no systray */
static const int showbar			= 1;	/* 0 means no bar */
static const int topbar				= 1;	/* 0 means bottom bar */
static const char *fonts[]			= { "Hack:size=9:antialias:true" };
static const char dmenufont[]			= "Hack:size=9";
static const char col_black[]			= "#151515";
static const char col_gray1[]			= "#000000";
static const char col_gray2[]			= "#bbbbbb";
static const char col_gray3[]			= "#eeeeee";
static const char col_carbon[]			= "#323232";
static const char *colors[][3]			= {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray2, col_black, col_gray1 },
	[SchemeSel]  = { col_gray3, col_carbon, col_carbon  },
};

/* tagging */
static const char *tags[] = { "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX" };
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact	= 0.55;	/* factor of master area size [0.05..0.95] */
static const int nmaster	= 1;    /* number of clients in master area */
static const int resizehints	= 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen	= 1;	/* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
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
static char dmenumon[2] 	= "0"; /* component of dmenucmd, manipulated in spawn() */
static const char dmenuhp[]	= "librewolf,gimp,steam,telegram-desktop,alacritty,pcmanfm,qbittorrent,chromium,thunderbird";
static const char *dmenucmd[]	= { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_black, "-nf", col_gray2, "-sb", col_carbon, "-sf", col_gray3, "-hp", dmenuhp, NULL };
static const char *termcmd[]	= { TERM, NULL };
static const char *browser[]	= { BROWSER, NULL };
static const char *tlg[]	= { "telegram-desktop", NULL };
static const char *email[]	= { "thunderbird", NULL };
static const char *discord[]	= { "vesktop", NULL };
static const char *steam[]	= { "steam", NULL };
static const char *lf[]		= { TERM, "-e", "lf", NULL };
static const char *zap[]	= { TERM, "-e", "nchat", "-d", "/home/joao/.config/nchat", NULL };
static const char *spot[]	= { TERM, "-e", "spotify_player", NULL };
static const char *torrent[]	= { "qbittorrent", NULL };
static const char *pcmanfm[]	= { "pcmanfm", NULL };
static const char *lock[]	= { "slock", NULL };
static const char *mutevol[]    = { "/usr/bin/wpctl", "set-mute", "@DEFAULT_AUDIO_SINK@", "toggle", NULL };
static const char *upvol[]      = { "/usr/bin/wpctl", "set-volume", "@DEFAULT_AUDIO_SINK@", "1%+", NULL };
static const char *downvol[]    = { "/usr/bin/wpctl", "set-volume", "@DEFAULT_AUDIO_SINK@", "1%-", NULL };
static const char *uplight[]	= { "xbacklight", "-inc", "2", NULL };
static const char *downlight[]	= { "xbacklight", "-dec", "2", NULL };

static const Key keys[] = {
	/* modifier		key        			function	argument */
	{ MODKEY,		XK_space,			spawn,		{.v = dmenucmd } },
	{ MODKEY,		XK_Return,			spawn,		{.v = termcmd } },
	{ MODKEY,		XK_b,				togglebar,	{0} },
	{ MODKEY,		XK_j,				focusstack,	{.i = +1 } },
	{ MODKEY,		XK_k,				focusstack,	{.i = -1 } },
	{ MODKEY,		XK_i,				incnmaster,	{.i = +1 } },
	{ MODKEY,		XK_d,				incnmaster,	{.i = -1 } },
	{ MODKEY,		XK_h,				setmfact,	{.f = -0.05} },
	{ MODKEY,		XK_l,				setmfact,	{.f = +0.05} },
	{ MODKEY|ShiftMask,	XK_Return,			zoom,		{0} },
	{ MODKEY,		XK_Tab,				view,		{0} },
	{ MODKEY,		XK_c,				killclient,	{0} },
	{ MODKEY|ControlMask,	XK_space,			setlayout,	{0} },
	{ MODKEY|ShiftMask,	XK_space,			togglefloating,	{0} },
	{ MODKEY|ShiftMask,	XK_f,				togglefullscr,	{0} },
	{ MODKEY,		XK_0,				view,		{.ui = ~0 } },
	{ MODKEY|ShiftMask,	XK_0,				tag,		{.ui = ~0 } },
	{ MODKEY,		XK_comma,			focusmon,	{.i = -1 } },
	{ MODKEY,		XK_period,			focusmon,	{.i = +1 } },
	{ MODKEY|ShiftMask,	XK_comma,			tagmon,		{.i = -1 } },
	{ MODKEY|ShiftMask,	XK_period,			tagmon,		{.i = +1 } },
	{ MODKEY,		XK_w,				spawn,		{.v = browser } },
	{ MODKEY,		XK_t,				spawn,		{.v = tlg } },
	{ MODKEY,		XK_e,				spawn,		{.v = email } },
	{ MODKEY,		XK_m,				spawn,		{.v = discord } },
	{ MODKEY,		XK_g,				spawn,		{.v = steam } },
	{ MODKEY,		XK_f,				spawn,		{.v = lf } },
	{ MODKEY,		XK_z,				spawn,		{.v = zap } },
	{ MODKEY,		XK_s,				spawn,		{.v = spot } },
	{ MODKEY,		XK_q,				spawn,		{.v = torrent } },
	{ MODKEY,		XK_p,				spawn,		{.v = pcmanfm } },
	{ MODKEY|ShiftMask,	XK_l,				spawn,		{.v = lock } },
	{ 0,			XF86XK_AudioMute,		spawn,		{.v = mutevol } },
	{ 0,			XF86XK_AudioLowerVolume,	spawn,		{.v = downvol } },
	{ 0,			XF86XK_AudioRaiseVolume,	spawn,		{.v = upvol } },
	{ 0,			XF86XK_MonBrightnessUp,		spawn,		{.v = uplight } },
	{ 0,			XF86XK_MonBrightnessDown,	spawn,		{.v = downlight } },
	{ 0,			XK_Print,			spawn,		SHCMD("scrot /home/joao/pix/ss/%Y-%m-%d_%H-%M-%S.png") },
	TAGKEYS(		XK_1,                      			0)
	TAGKEYS(		XK_2,                      			1)
	TAGKEYS(		XK_3,                      			2)
	TAGKEYS(		XK_4,                      			3)
	TAGKEYS(		XK_5,                      			4)
	TAGKEYS(		XK_6,                      			5)
	TAGKEYS(		XK_7,                      			6)
	TAGKEYS(		XK_8,                      			7)
	TAGKEYS(		XK_9,                      			8)
	{ MODKEY|ShiftMask,	XK_q,				quit,          	{0} },
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
