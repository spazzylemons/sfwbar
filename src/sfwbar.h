#if !defined(__SFWBAR_H__)
#define __SFWBAR_H__

#include <glib.h>
#include <gtk/gtk.h>
#include <json.h>
#include "wlr-foreign-toplevel-management-unstable-v1.h"

#define MAX_BUTTON 7

struct wt_window {
  GtkWidget *button;
  GtkWidget *switcher;
  gchar *title;
  gchar *appid;
  gchar *output;
  gint64 pid;
  gint64 wid;
  gpointer uid;
  guchar state;
};

struct rect {
  guint x,y,w,h;
};

struct scan_var {
  GRegex *regex;
  gchar *json;
  gchar *str;
  double val;
  double pval;
  gint64 time;
  gint64 ptime;
  gint count;
  gint multi;
  guchar type;
  guchar status;
  struct scan_file *file;
};

struct scan_file {
  gchar *fname;
  gint flags;
  guchar source;
  time_t mtime;
  GList *vars;
};

struct layout_action {
  guchar cond;
  guchar ncond;
  guchar type;
  gchar *command;
};

struct layout_widget {
  GtkWidget *widget;
  GtkWidget *lobject;
  gchar *style;
  gchar *css;
  gchar *value;
  struct layout_action action[MAX_BUTTON];
  gchar *eval;
  gint64 interval;
  gint64 next_poll;
  gint wtype;
  gint dir;
  struct rect rect;
};

extern gchar *expr_token[];

void action_exec ( GtkWidget *, struct layout_action *, GdkEvent *,
    struct wt_window *);
void action_free ( struct layout_action *action, GObject *old );
void action_function_add ( gchar *name, GList *actions );
void action_function_exec ( gchar *name, GtkWidget *w, GdkEvent *ev,
    struct wt_window *win );

void sway_ipc_init ( void );
gboolean sway_ipc_active ( void );
gchar *sway_ipc_poll ( gint sock, gint32 *etype );
int sway_ipc_open (int to);
int sway_ipc_send ( gint sock, gint32 type, gchar *command );
void sway_ipc_command ( gchar *cmd, ... );
int sway_ipc_subscribe ( gint sock );
gboolean sway_ipc_event ( GIOChannel *, GIOCondition , gpointer );
void sway_ipc_rescan ( void );

void place_window ( gint64 wid, gint64 pid );
void placer_config ( gint xs, gint ys, gint xo, gint yo, gboolean pid );

void taskbar_init ( struct layout_widget * );
void taskbar_invalidate ( void );
void taskbar_set_options ( gboolean, gboolean, gboolean );
void taskbar_update ( void );
void taskbar_window_init ( struct wt_window *win );
void taskbar_set_label ( struct wt_window *win, gchar *title );

struct wt_window *wintree_window_init ( void );
struct wt_window *wintree_from_id ( gpointer id );
struct wt_window *wintree_from_pid ( gint64 pid );
void wintree_window_append ( struct wt_window *win );
void wintree_window_delete ( gpointer id );
void wintree_set_focus ( gpointer id );
void wintree_set_active ( gchar *title );
void wintree_focus ( gpointer id );
void wintree_close ( gpointer id );
void wintree_minimize ( gpointer id );
void wintree_maximize ( gpointer id );
void wintree_unminimize ( gpointer id );
void wintree_unmaximize ( gpointer id );
gpointer wintree_get_focus ( void );
gchar *wintree_get_active ( void );
gboolean wintree_is_focused ( gpointer id );
GList *wintree_get_list ( void );

void wayland_init ( GtkWindow * );
void foreign_toplevel_activate ( gpointer tl );

gboolean window_hide_event ( struct json_object *obj );

struct layout_widget *config_parse ( gchar * );
void config_pipe_read ( gchar *command );
void config_string ( gchar *string );

char *expr_parse ( gchar *expr_str, guint * );

gboolean switcher_event ( struct json_object *obj );
void switcher_invalidate ( void );
void switcher_update ( void );
void switcher_set_label ( struct wt_window *win, gchar *title );
void switcher_window_init ( struct wt_window *win);
void switcher_config ( gint, gchar *, gint, gboolean, gboolean);

void pager_init ( GtkWidget * );
void pager_set_preview ( gboolean pv );
void pager_set_numeric ( gboolean pn );
void pager_add_pin ( gchar *pin );
void pager_update ( void );

void sni_init ( GtkWidget *w );
void sni_update ( void );

GtkWidget *layout_menu_get ( gchar *name );
void layout_menu_add ( gchar *name, GtkWidget *menu );
void layout_menu_remove ( gchar *name );
struct layout_widget *layout_widget_new ( void );
void layout_menu_popup ( GtkWidget *, GtkWidget *, GdkEvent *, gpointer );
gpointer layout_scanner_thread ( gpointer data );
GtkWidget *layout_widget_config ( struct layout_widget *lw, GtkWidget *parent,
    GtkWidget *sibling );
void layout_widget_attach ( struct layout_widget *lw );
void layout_widget_free ( struct layout_widget *lw );
void layout_widgets_update ( GMainContext * );
void widget_set_css ( GtkWidget * );
gboolean widget_menu_action ( GtkWidget *widget, struct layout_action *action );

GtkWidget *flow_grid_new( gboolean limit );
void flow_grid_set_rows ( GtkWidget *cgrid, gint rows );
void flow_grid_set_cols ( GtkWidget *cgrid, gint cols );
void flow_grid_attach ( GtkWidget *cgrid, GtkWidget *w );
void flow_grid_pad ( GtkWidget *cgrid );
void flow_grid_clean ( GtkWidget *cgrid );

void scanner_var_attach ( gchar *name, struct scan_var *var );
void scanner_expire ( void );
int scanner_reset_vars ( GList *var_list );
int scanner_glob_file ( struct scan_file *file );
char *scanner_get_string ( gchar *name );
double scanner_get_numeric ( gchar *name );

gchar *gdk_monitor_get_xdg_name ( GdkMonitor *monitor );
void set_monitor ( gchar *mon_name );
void set_layer ( gchar *layer );
void bar_set_size ( gchar *size );
gchar *bar_get_output ( void );

gchar *get_xdg_config_file ( gchar *fname, gchar *extra );
gchar *json_string_by_name ( struct json_object *obj, gchar *name );
gint64 json_int_by_name ( struct json_object *obj, gchar *name, gint64 defval);
gboolean json_bool_by_name ( struct json_object *obj, gchar *name, gboolean defval);
gdouble json_double_by_name ( struct json_object *obj, gchar *name, gdouble defval);
int md5_file( gchar *path, guchar output[16] );
void str_assign ( gchar **dest, gchar *source );
struct rect parse_rect ( struct json_object *obj );
gint get_toplevel_dir ( void );
guint str_nhash ( gchar *str );
gboolean str_nequal ( gchar *str1, gchar *str2 );

void scale_image_set_image ( GtkWidget *widget, gchar *image, gchar *extra );
GtkWidget *scale_image_new();
int scale_image_update ( GtkWidget *widget );
void scale_image_set_pixbuf ( GtkWidget *widget, GdkPixbuf * );

#define SCAN_VAR(x) ((struct scan_var *)x)
#define AS_WINDOW(x) ((struct wt_window *)(x))

enum {
  WS_FOCUSED =    1<<0,
  WS_MINIMIZED =  1<<1,
  WS_MAXIMIZED =  1<<2,
  WS_FULLSCREEN = 1<<3
};

enum {
  SV_ADD = 1,
  SV_PRODUCT = 2,
  SV_REPLACE = 3,
  SV_FIRST = 4
};

enum {
  SO_FILE = 0,
  SO_EXEC = 1
};

enum {
  VP_REGEX = 0,
  VP_JSON = 1,
  VP_GRAB = 2
};

enum {
  VF_CHTIME = 1,
  VF_NOGLOB = 2
};

enum {
  ACT_EXEC = 1,
  ACT_MENU = 2,
  ACT_CLEAR = 3,
  ACT_PIPE = 4,
  ACT_SWAY = 5,
  ACT_SWIN = 6,
  ACT_CONF = 7,
  ACT_MONITOR = 8,
  ACT_LAYER = 9,
  ACT_BARSIZE = 10,
  ACT_FUNC = 90,
  ACT_FOCUS = 101,
  ACT_CLOSE = 102,
  ACT_MIN = 103,
  ACT_MAX = 104,
  ACT_UNMIN = 105,
  ACT_UNMAX = 106,
};

enum {
  G_TOKEN_TIME    = G_TOKEN_LAST + 1,
  G_TOKEN_MIDW    = G_TOKEN_LAST + 2,
  G_TOKEN_EXTRACT = G_TOKEN_LAST + 3,
  G_TOKEN_DISK    = G_TOKEN_LAST + 4,
  G_TOKEN_VAL     = G_TOKEN_LAST + 5,
  G_TOKEN_STRW    = G_TOKEN_LAST + 6,
  G_TOKEN_ACTIVE  = G_TOKEN_LAST + 7,
  G_TOKEN_PAD     = G_TOKEN_LAST + 8
};

#endif
