#ifndef __EXPRN_H__
#define __EXPRN_H__

#include <gtk/gtk.h>
#include "vm/value.h"
#include "vm/expr.h"

enum expr_instruction_t {
  EXPR_OP_IMMEDIATE,
  EXPR_OP_JZ,
  EXPR_OP_JMP,
  EXPR_OP_CACHED,
  EXPR_OP_VARIABLE,
  EXPR_OP_FUNCTION
};

typedef struct {
  guint8 *ip;
  guint8 *code;
  gsize len;
  GArray *stack;
  gint max_stack;
  gboolean use_cached;
  guint16 wstate;
  GtkWidget *widget;
  GdkEvent *event;
  expr_cache_t *expr;
} vm_t;

typedef value_t (*vm_func_t)(vm_t *vm, value_t params[], gint np);

typedef struct {
  vm_func_t function;
  gboolean deterministic;
} vm_function_t;

#define vm_param_check_np(vm, np, n, fname) { if(np!=n) { return value_na; } }
#define vm_param_check_np_range(vm, np, min, max, fname) { if(np<min || np>max) { return value_na; } }
#define vm_param_check_string(vm, p, n, fname) { if(!value_like_string(p[n])) { return value_na; } }
#define vm_param_check_numeric(vm, p, n, fname) { if(!value_like_numeric(p[n])) { return value_na; } }

GByteArray *parser_expr_compile ( gchar *expr );
value_t vm_expr_eval ( expr_cache_t *expr );
gchar *expr_vm_result_to_string ( vm_t *vm );
gint expr_vm_get_func_params ( vm_t *vm, value_t *params[] );

void vm_func_init ( void );
void vm_func_add ( gchar *name, vm_func_t func, gboolean deterministic );
vm_function_t *vm_func_lookup ( gchar *name );
gboolean vm_func_remove ( gchar *name );

void vm_run_action ( gchar *func, gchar *expr1, gchar *expr2, GtkWidget *w, GdkEvent *e );
#endif
