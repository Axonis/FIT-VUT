#ifndef _DYNAMIC_STACK_H
#define _DYNAMIC_STACK_H

#include <sys/types.h> 
#include <stdbool.h>

#include "ial.h"

typedef struct 
{
	int type;
	data_t*	symbol;
} expr_t;


typedef expr_t obj_t;
extern const size_t INIT_SIZE;  

typedef struct {
    obj_t   *elem;
    long int     top;
    size_t  size;
} dstack_t;

dstack_t dstack_ctor(void);
dstack_t dstack_clear(dstack_t *const);
bool dstack_empty(const dstack_t *const );
obj_t *dstack_top(const dstack_t *const);
void dstack_pop(dstack_t *const);
void dstack_push(dstack_t *const, obj_t);
dstack_t dstack_dtor(dstack_t *const);
bool dstack_replace(dstack_t *const, long, obj_t *, long);
long dstack_elem_count(const dstack_t *const stack);

bool dstack_add_handle_symbol(dstack_t *, unsigned );

#endif

