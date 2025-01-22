#include "push_swap.h"


//---------------------TOOLS--------------------------------//
t_list *last_list(t_list *p)
{
    while(p->next)
        p = p->next;
    return (p);
}
void add_back(t_list *list,t_list *node)
{
    t_list *last;

    last = last_list(list);
    last->next = node;
}
void add_null(t_list *node, t_list *stack)
{
    while (stack->next != node)
        stack = stack->next;
    stack->next = NULL;
}
//--------------------SWAP----------------------------------//
void sa(t_list **stack)
{
    t_list *tmp1;
    t_list *tmp2;

    tmp1 = *stack;
    tmp2 = (*stack)->next;

    tmp1->next = tmp2->next;
    tmp2->next = tmp1;
    *stack = tmp2;
    write(1,"sa\n",2);
}
void sb(t_list **stack)
{
    t_list *tmp1;
    t_list *tmp2;

    tmp1 = *stack;
    tmp2 = (*stack)->next;

    tmp1->next = tmp2->next;
    tmp2->next = tmp1;
    *stack = tmp2;
    write(1,"sb",2);
}
void ss(t_list **stack_A, t_list **stack_B)
{
    sb(stack_B);
    sa(stack_A);
    write(1,"ss",2);
}
//-----------------------PUSH----------------------------//
void pa(t_list **stack_A, t_list **stack_B)
{
    t_list *b;
    t_list *a;

    b = *stack_B;

    *stack_B = (*stack_B)->next;
    b->next = *stack_A;
    *stack_A = b;
    write(1,"pa",2);
}
void pb(t_list **stack_A, t_list **stack_B)
{
    t_list *b;
    t_list *a;

    b = *stack_A;

    *stack_A = (*stack_A)->next;
    b->next = *stack_B;
    *stack_B = b;
    write(1,"pb",2);
}
//---------------------ROTATE------------------------------//
void ra(t_list **stack_A)
{
    t_list *tmp;

    tmp = *stack_A;
    *stack_A = (*stack_A)->next;
    tmp->next = NULL;
    add_back(*stack_A,tmp);
    write(1,"ra",2);
}
void rb(t_list **stack_B)
{
    t_list *tmp;

    tmp = *stack_B;
    *stack_B = (*stack_B)->next;
    tmp->next = NULL;
    add_back(*stack_B,tmp);
    write(1,"rb",2);
}
void rr(t_list **stack_A, t_list **stack_B)
{
    ra(stack_A);
    rb(stack_B);
    write(1,"rr",2);
}
//------------------------REVERSE ROTATE-------------------------------------//
void rra(t_list **stack_A)
{
    t_list *last;

    last = last_list(*stack_A);
    add_null(last,*stack_A);
    last->next = *stack_A;
    *stack_A = last;
    write(1,"rr",3);
}
void rrb(t_list **stack_B)
{
    t_list *last;

    last = last_list(*stack_B);
    add_null(last,*stack_B);
    last->next = *stack_B;
    *stack_B = last;
}
void    rrr(t_list **stack_B,t_list **stack_A)
{
    rrb(stack_B);
    rra(stack_A);
}
