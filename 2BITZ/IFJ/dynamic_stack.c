#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "dynamic_stack.h"
#include "scanner.h"

const size_t INIT_SIZE = 512;
const double RESIZE_FACTOR = 0.7;

static dstack_t dstack_resize(dstack_t *const, size_t);

dstack_t dstack_ctor(void)
{
    dstack_t stack;

    stack.elem = calloc(sizeof(obj_t), INIT_SIZE);
    if (stack.elem == NULL)
        fprintf(stderr, "%s: Allocation of %zu elements for stack failed!\n", __func__, INIT_SIZE);
    stack.size = INIT_SIZE;
    stack.top = -1;

    return stack;
}

dstack_t dstack_clear(dstack_t *const stack)
{
    if (stack)
    {
        stack->top = -1;    
    }
    return *stack;
}

bool dstack_empty(const dstack_t *const stack)
{
    if (stack)
       return stack->top == -1; 
    return true;
}

// HACK HACK what to do with 'o'?
obj_t *dstack_top(const dstack_t *const stack)
{
    if (stack && stack->top >= 0)  
       return &stack->elem[stack->top];
    return NULL;
}

void dstack_pop(dstack_t *const stack)
{
    if (stack && stack->top >= 0)
       --stack->top;    
}

void dstack_push(dstack_t *const stack, obj_t c)
{
    if (stack)
    {
       if (stack->top >= stack->size*RESIZE_FACTOR) 
           *stack = dstack_resize(stack, stack->size + stack->size);
       stack->elem[++stack->top] = c;  
    }
}

static dstack_t dstack_resize(dstack_t *const stack, size_t sz)
{
   obj_t *s = realloc(stack->elem, sz*sizeof(obj_t));

   if (s != NULL)
   {
       stack->elem = s;
       stack->size = sz;
   }
   else
       fprintf(stderr, "%s: Reallocation of %zu bytes for stack failed!\n", __func__, sz);
   return *stack;
}

dstack_t dstack_dtor(dstack_t *const stack)
{
    if (stack)
    {
        if (stack->elem != NULL)
            free(stack->elem);
        stack->elem = NULL;
        stack->size = 0;
        stack->top = -1;
    }
    return *stack;
}

long dstack_elem_count(const dstack_t *const stack)
{
    return (stack == NULL) ? 0 : stack->top+1;
}

bool dstack_replace(dstack_t *stack, long pos, obj_t *new, long num)
{   
    if (stack == NULL || new == NULL || pos < 0L || num <= 0L)
            return false;
    
    long len = dstack_elem_count(stack); 
    
    if (len && num && pos <= len)
    {   
        if (len + num > (long)stack->size)
            *stack = dstack_resize(stack, stack->size + stack->size);
        for (long i = len; i >= pos; i--)
            stack->elem[i+num] = stack->elem[i];
        for (long i = 0; i != num; i++)
            stack->elem[pos+i] = new[i]; 
        stack->top += num;
        return true;
    }            
    return false;
}

bool dstack_add_handle_symbol(dstack_t *stack, unsigned symbol)
{
    if (stack == NULL )
        return false;

    long end = stack->top;

    for (; end != -1; end--)
    {
        if (stack->elem[end].type == symbol)
        {
           // we can hack maybe and add just '<' behind the symbol 
           expr_t left_sharp = {.type = '<'};
           // insert one position behind the symbol in stack left sharp
           dstack_replace(stack, end+1, &left_sharp, 1);
           return true;
        }
    
    }
    return false;
}

