#ifndef __ACTION_H__
#define __ACTION_H__

#include "wintree.h"
#include "vm/expr.h"

typedef struct user_action {
  guint8 *code;
  gint len;
  guchar cond;
  guchar ncond;
  expr_cache_t *command;
  expr_cache_t *addr;
  const gchar *id;
} action_t;

void action_exec ( GtkWidget *, action_t *, GdkEvent *, window_t *, guint16 *);
action_t *action_new ( void );
void action_free ( action_t *, GObject *);
action_t *action_dup ( action_t *src );
void action_function_add ( gchar *, action_t *);
void action_function_exec ( gchar *, GtkWidget *, GdkEvent *, window_t *,
    guint16 *);
void action_trigger_add ( action_t *action, gchar *trigger );
action_t *action_trigger_lookup ( const gchar *trigger );
void action_lib_init ( void );

#endif
