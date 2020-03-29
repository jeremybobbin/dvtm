/* valid curses attributes are listed below they can be ORed
 *
 * A_NORMAL        Normal display (no highlight)
 * A_STANDOUT      Best highlighting mode of the terminal.
 * A_UNDERLINE     Underlining
 * A_REVERSE       Reverse video
 * A_BLINK         Blinking
 * A_DIM           Half bright
 * A_BOLD          Extra bright or bold
 * A_PROTECT       Protected mode
 * A_INVIS         Invisible or blank mode
 */

enum {
	DEFAULT,
	BLUE,
};

static Color colors[] = {
	[DEFAULT] = { .fg = -1,         .bg = -1, .fg256 = -1, .bg256 = -1, },
	[BLUE]    = { .fg = COLOR_BLUE, .bg = -1, .fg256 = 68, .bg256 = -1, },
};

#define COLOR(c)        COLOR_PAIR(colors[c].pair)
/* curses attributes for the currently focused window */
#define SELECTED_ATTR   (COLOR(BLUE) | A_NORMAL)
/* curses attributes for normal (not selected) windows */
#define NORMAL_ATTR     (COLOR(DEFAULT) | A_NORMAL)
/* curses attributes for a window with pending urgent flag */
#define URGENT_ATTR     NORMAL_ATTR
/* curses attributes for the status bar */
#define BAR_ATTR        (COLOR(BLUE) | A_NORMAL)
/* characters for beginning and end of status bar message */
#define BAR_BEGIN       '['
#define BAR_END         ']'
/* status bar (command line option -s) position */
#define BAR_POS         BAR_TOP /* BAR_BOTTOM, BAR_OFF */
/* whether status bar should be hidden if only one client exists */
#define BAR_AUTOHIDE    true
/* master width factor [0.1 .. 0.9] */
#define MFACT 0.5
/* number of clients in master area */
#define NMASTER 1
/* scroll back buffer size in lines */
#define SCROLL_HISTORY 500
/* printf format string for the tag in the status bar */
#define TAG_SYMBOL   "[%s]"
/* curses attributes for the currently selected tags */
#define TAG_SEL      (COLOR(BLUE) | A_BOLD)
/* curses attributes for not selected tags which contain no windows */
#define TAG_NORMAL   (COLOR(DEFAULT) | A_NORMAL)
/* curses attributes for not selected tags which contain windows */
#define TAG_OCCUPIED (COLOR(BLUE) | A_NORMAL)
/* curses attributes for not selected tags which with urgent windows */
#define TAG_URGENT (COLOR(BLUE) | A_NORMAL | A_BLINK)

const char tags[][8] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

#include "tile.c"
#include "grid.c"
#include "bstack.c"
#include "fullscreen.c"

/* by default the first layout entry is used */
static Layout layouts[] = {
	{ "[]=", tile },
	{ "+++", grid },
	{ "TTT", bstack },
	{ "[ ]", fullscreen },
};

#define MOD  CTRL('g')
#define TAGKEYS(KEY,TAG) \
	{ NORMAL, { KEY,               }, { view,           { tags[TAG] }               } }, \
	{ NORMAL, { 'v', KEY,          }, { toggleview,     { tags[TAG] }               } }, \
	{ NORMAL, { 't', KEY,          }, { toggletag,      { tags[TAG] }               } },

/* you can specifiy at most 3 arguments */
static KeyBinding bindings[] = {
	{ INSERT, { MOD,               }, { togglemode,     { NULL }                          } },
	{ NORMAL, { 'i',               }, { togglemode,     { NULL }                          } },
	{ NORMAL, { 'C',               }, { create,         { NULL }                          } },
	{ NORMAL, { 'c',               }, { create,         { NULL, NULL, "$CWD" }            } },
	{ NORMAL, { '-',               }, { create,         { "$FILE_BROWSER", NULL, "$CWD" } } },
	{ NORMAL, { 'x', 'x',          }, { killclient,     { NULL }                          } },
	{ NORMAL, { 'j',               }, { focusnext,      { NULL }                          } },
	{ NORMAL, { 'J',               }, { focusdown,      { NULL }                          } },
	{ NORMAL, { 'K',               }, { focusup,        { NULL }                          } },
	{ NORMAL, { 'H',               }, { focusleft,      { NULL }                          } },
	{ NORMAL, { 'L',               }, { focusright,     { NULL }                          } },
	{ NORMAL, { 'k',               }, { focusprev,      { NULL }                          } },
	{ NORMAL, { 'f',               }, { setlayout,      { "[]=" }                         } },
	{ NORMAL, { 'g',               }, { setlayout,      { "+++" }                         } },
	{ NORMAL, { 'b',               }, { setlayout,      { "TTT" }                         } },
	{ NORMAL, { 'm',               }, { setlayout,      { "[ ]" }                         } },
	{ NORMAL, { ' ',               }, { setlayout,      { NULL }                          } },
	{ NORMAL, { 'I',               }, { incnmaster,     { "+1" }                          } },
	{ NORMAL, { 'D',               }, { incnmaster,     { "-1" }                          } },
	{ NORMAL, { 'h',               }, { setmfact,       { "-0.05" }                       } },
	{ NORMAL, { 'l',               }, { setmfact,       { "+0.05" }                       } },
	{ NORMAL, { '.',               }, { toggleminimize, { NULL }                          } },
	{ NORMAL, { 's',               }, { togglebar,      { NULL }                          } },
	{ NORMAL, { 'S',               }, { togglebarpos,   { NULL }                          } },
	{ NORMAL, { 'M',               }, { togglemouse,    { NULL }                          } },
	{ NORMAL, { '\n',              }, { zoom,           { NULL }                          } },
	{ NORMAL, { '\r',              }, { zoom,           { NULL }                          } },
	{ NORMAL, { 'q', 'q',          }, { quit,           { NULL }                          } },
	{ NORMAL, { 'a',               }, { togglerunall,   { NULL }                          } },
	{ NORMAL, { CTRL('L'),         }, { redraw,         { NULL }                          } },
	{ NORMAL, { 'r',               }, { redraw,         { NULL }                          } },
	{ NORMAL, { 'e',               }, { copymode,       { "dvtm-editor" }                 } },
	{ NORMAL, { 'E',               }, { copymode,       { "dvtm-pager", "G" }             } },
	{ NORMAL, { '/',               }, { copymode,       { "dvtm-pager", "/" }             } },
	{ NORMAL, { 'p',               }, { paste,          { NULL }                          } },
	{ NORMAL, { KEY_PPAGE,         }, { scrollback,     { "-1" }                          } },
	{ NORMAL, { KEY_NPAGE,         }, { scrollback,     { "1"  }                          } },
	{ NORMAL, { '?',               }, { create,         { "man dvtm", "dvtm help" }       } },
	{ NORMAL, { MOD,               }, { send,           { (const char []){MOD, 0} }       } },
	{ NORMAL, { KEY_SPREVIOUS,     }, { scrollback,     { "-1" }                          } },
	{ NORMAL, { KEY_SNEXT,         }, { scrollback,     { "1"  }                          } },
	{ NORMAL, { '0',               }, { view,           { NULL }                          } },
	{ NORMAL, { '\t',              }, { viewprevtag,    { NULL }                          } },
	{ NORMAL, { ')',               }, { tag,            { NULL }                          } },
	{ NORMAL, { '!',               }, { tag,            { tags[0] }                       } },
	{ NORMAL, { '@',               }, { tag,            { tags[1] }                       } },
	{ NORMAL, { '#',               }, { tag,            { tags[2] }                       } },
	{ NORMAL, { '$',               }, { tag,            { tags[3] }                       } },
	{ NORMAL, { '%',               }, { tag,            { tags[4] }                       } },
	{ NORMAL, { '^',               }, { tag,            { tags[5] }                       } },
	{ NORMAL, { '&',               }, { tag,            { tags[6] }                       } },
	{ NORMAL, { '*',               }, { tag,            { tags[7] }                       } },
	{ NORMAL, { '(',               }, { tag,            { tags[8] }                       } },
	TAGKEYS( '1',                              0)
	TAGKEYS( '2',                              1)
	TAGKEYS( '3',                              2)
	TAGKEYS( '4',                              3)
	TAGKEYS( '5',                              4)
	TAGKEYS( '6',                              5)
	TAGKEYS( '7',                              6)
	TAGKEYS( '8',                              7)
	TAGKEYS( '9',                              8)
};

static const ColorRule colorrules[] = {
	{ "", A_NORMAL, &colors[DEFAULT] }, /* default */
};

/* possible values for the mouse buttons are listed below:
 *
 * BUTTON1_PRESSED          mouse button 1 down
 * BUTTON1_RELEASED         mouse button 1 up
 * BUTTON1_CLICKED          mouse button 1 clicked
 * BUTTON1_DOUBLE_CLICKED   mouse button 1 double clicked
 * BUTTON1_TRIPLE_CLICKED   mouse button 1 triple clicked
 * BUTTON2_PRESSED          mouse button 2 down
 * BUTTON2_RELEASED         mouse button 2 up
 * BUTTON2_CLICKED          mouse button 2 clicked
 * BUTTON2_DOUBLE_CLICKED   mouse button 2 double clicked
 * BUTTON2_TRIPLE_CLICKED   mouse button 2 triple clicked
 * BUTTON3_PRESSED          mouse button 3 down
 * BUTTON3_RELEASED         mouse button 3 up
 * BUTTON3_CLICKED          mouse button 3 clicked
 * BUTTON3_DOUBLE_CLICKED   mouse button 3 double clicked
 * BUTTON3_TRIPLE_CLICKED   mouse button 3 triple clicked
 * BUTTON4_PRESSED          mouse button 4 down
 * BUTTON4_RELEASED         mouse button 4 up
 * BUTTON4_CLICKED          mouse button 4 clicked
 * BUTTON4_DOUBLE_CLICKED   mouse button 4 double clicked
 * BUTTON4_TRIPLE_CLICKED   mouse button 4 triple clicked
 * BUTTON_SHIFT             shift was down during button state change
 * BUTTON_CTRL              control was down during button state change
 * BUTTON_ALT               alt was down during button state change
 * ALL_MOUSE_EVENTS         report all button state changes
 * REPORT_MOUSE_POSITION    report mouse movement
 */

#ifdef NCURSES_MOUSE_VERSION
# define CONFIG_MOUSE /* compile in mouse support if we build against ncurses */
#endif

#define ENABLE_MOUSE true /* whether to enable mouse events by default */

#ifdef CONFIG_MOUSE
static Button buttons[] = {
	{ BUTTON1_CLICKED,        { mouse_focus,      { NULL  } } },
	{ BUTTON1_DOUBLE_CLICKED, { mouse_fullscreen, { "[ ]" } } },
	{ BUTTON2_CLICKED,        { mouse_zoom,       { NULL  } } },
	{ BUTTON3_CLICKED,        { mouse_minimize,   { NULL  } } },
};
#endif /* CONFIG_MOUSE */

static Cmd commands[] = {
	/* create [cmd]: create a new window, run `cmd` in the shell if specified */
	{ "create", { create,	{ NULL } } },
	/* focus <win_id>: focus the window whose `DVTM_WINDOW_ID` is `win_id` */
	{ "focus",  { focusid,	{ NULL } } },
	/* tag <win_id> <tag> [tag ...]: add +tag, remove -tag or set tag of the window with the given identifier */
	{ "tag",    { tagid,	{ NULL } } },
};

/* gets executed when dvtm is started */
static Action actions[] = {
	{ create, { NULL } },
};

static char const * const keytable[] = {
	/* add your custom key escape sequences */
};
