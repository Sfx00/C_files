#include "push_swap.h"

//main function : is responsable of calling function.
//stack function : responsable for storing argument in stack and calling other funtion to help.
//error function : free all memory blocks that allocated and write error and exit with 1.
//check_input function : the arguments is numbers charaters and converte from string to long and check if nb is in range of integer.
//ft_atol : convert from string to long.
// addt_stack : function that add new node into struct(stack) with value and check if that value are already exist.
// create_node : it just create a node base in what you give her (integer);
// last_node : return address of last node and also check if content is in one of the nodes.


size_t  ft_strlen(const char *s)
{
    size_t i;

    i = 0;
    while(s[i])
        i++;
    return(i);
}
void	*free_strings(char **s)
{
	int	i;

	i = 0;
    if (!s)
    {
        return(NULL);
    }

	while(s[i])
	{
		free(s[i]);
		i++;
	}
	free(s);
	return (NULL);
}
void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*p;

	i = 0;
	p = (unsigned char *)s;
	while (i < n)
	{
		p[i] = (unsigned char)c;
		i++;
	}
	return (s);
}
void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*p;

	p = (void *)malloc(nmemb * size);
	if (!p)
	{
		return (NULL);
	}
	ft_memset(p,0,nmemb * size);
	return (p);
}
static int	lenw(const char *s, char c)
{
	int	len;
	int	i;

	len = 0;
	i = 0;
	if (s == NULL)
		return (len);
	while (s[i] != c && s[i] != '\0')
	{
		len++;
		i++;
	}
	return (len);
}
static char	*create(const char *s, int i, char c)
{
	char	*p;
	int		x;

	x = 0;
	if (s == NULL)
		return (NULL);
	p = (char *)malloc((lenw(s + i, c) + 1) * sizeof(char));
	if (p == NULL)
		return (NULL);
	while (s[i] != c && s[i] != '\0')
	{
		p[x++] = s[i++];
	}
	p[x] = '\0';
	return (p);
}
static int	counter(char const *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (s == NULL)
		return (count);
	while (s[i] != '\0')
	{
		while (s[i] == c && s[i] != '\0')
			i++;
		if (s[i] != '\0')
			count++;
		while (s[i] != c && s[i] != '\0')
			i++;
	}
	return (count);
}
char	**ft_split(char const *s, char c)
{
	char	**p;
	int		arr[2];

	arr[0] = 0;
	arr[1] = 0;
	if (!s)
		return (NULL);
	p = (char **)ft_calloc((counter(s, c) + 1),sizeof(char *));
	if (p == NULL)
		return (NULL);
	while (s[arr[0]] != '\0')
	{
		while (s[arr[0]] == c && s[arr[0]] != '\0')
			arr[0]++;
		if (s[arr[0]] != '\0')
		{
			p[arr[1]] = create(s, arr[0], c);
			if (!p[arr[1]])
				return (free_strings(p));
			arr[1]++;
		}
		while (s[arr[0]] != c && s[arr[0]] != '\0')
			arr[0]++;
	}
	return (p);
}
/*--------------------------------------------------*/
void    set_level(t_list *stack)
{
    
}
int check_sorted(t_list *stack)
{
    int nb;

    nb = stack->number;
    while(stack->next)
    {
        stack = stack->next;
        if(nb > stack->number)
        {
            return(0);
        }
        nb = stack->number;
    }
    return(1);
}
int stack_size(t_list *stack)
{
    int size;
    
    size = 0;
    while(stack)
    {
        stack = stack->next;
        size++;
    }
    return(size);
}
void error(char **buffer, t_list **stack)
{
    t_list *tmp;
    t_list *p;

    if(buffer)
    {
       free_strings(buffer);
    }
    if(stack)
    {
        tmp = *stack;
        while(tmp)
        {
            p = tmp->next;
            free(tmp);
            tmp = p;
        }
    }
    write(2,"Error\n",6);
    exit(1);
}
long    ft_atol(char *str)
{
    long    r;
    int i, d;

    i = 0;
    d = 0;
    r = 0;
    if(str[i] ==  '-' || str[i] == '+')
    {
        if(str[i] == '-')
            d = 1;
        i++;
    }
    while(str[i] && (str[i] >= '0' && str[i] <= '9'))
    {
        r = r * 10 + (str[i] - '0');
        i++;
    }
    if(d == 1)
        return(-r);
    return(r);
}
int check_input(char *buffer, long *nb)
{
    int i;
    size_t lent;

    i = 0;
    lent = ft_strlen(buffer);
    if(lent > 11)
        return(0);
    if((buffer[i] == '-' || buffer[i] == '+') && buffer[i + 1])
        i++;
    while(buffer[i] && (buffer[i] >= '0' && buffer[i] <= '9'))
        i++;
    if(buffer[i])
        return(0);
    *nb = ft_atol(buffer);
    if(*nb > INT_MAX || *nb < INT_MIN)
        return(0);
    return(1);
}
t_list *create_node(int nb)
{
    t_list *p;

    p = (t_list *)ft_calloc(2,sizeof(t_list));
    if(!p)
        return(NULL);
    p->number = nb;
    p->next = NULL;
    return(p);
}
t_list *last_node(t_list *p, int nb, int *flag)
{
    while(p->next)
    {
        if(p->number == nb)
            *flag = 1; 
        p = p->next;
    }
    if(p->number == nb)
        *flag = 1;
    return (p);
}
void addt_stack(int nb, t_list **stack, int * flag)
{
    t_list *p;

    if(*stack)
    {
        p = last_node(*stack,nb,flag);
        p->next = create_node(nb);
    }
    else
        *stack = create_node(nb);
}
void stack_check(char **av, t_list **stack)
{
    int i,x,flag ;
    char **buffer;
    long    nb;

    i = 0;
    flag = 0;
    while(av[i])
    {
        buffer = ft_split(av[i], ' ');
        if(!*buffer)
            error(buffer,stack);
        x = 0;
        while(buffer[x] && check_input(buffer[x],&nb) == 1 && flag == 0)
        {
            addt_stack(nb,stack,&flag);
            x++;
        }
        if(buffer[x] || flag == 1)
            error(buffer,stack);
        free_strings(buffer);
        i++;
    }
}
void free_stack(t_list **stack)
{
    t_list *tmp = *stack;
    while(tmp)
    {
        t_list *p = tmp->next;
        free(tmp);
        tmp = p;
    }
    *stack = NULL;
}
t_list * max(t_list *list)
{
    int nb;
    t_list *save;

    nb = list->number;
    save = list;
    while(list->next)
    {
        list = list->next;
        if(nb < list->number)
        {
            nb = list->number;
            save = list;
        }
    }
    return(save);
}

void sort_3(t_list **stack)
{
    t_list *max_nb;

    max_nb = max(*stack);
    
    if(max_nb == *stack)
        ra(stack);
    else if(max_nb == (*stack)->next)
        rra(stack);
    if((*stack)->number > (*stack)->next->number)
        sa(stack);
}
void rocket_sort(t_list **a)
{
    t_list *b;

    b = NULL;
    //a --> b
    while (stack_size(*a) > 3)
        push_to_b(a);
    //sort_3

    //b --> a
    //min at the head
}
void sort_stack(t_list **stack)
{
    int size;
 
    size = stack_size(*stack);
    if(size == 2)
        sa(stack);
    else if(size == 3)
        sort_3(stack);
    else
        rocket_sort(stack);
    
}







int main(int ac, char *av[])
{
    t_list *head;
 
    head = NULL;
    if(ac == 1)
        error(NULL,NULL);
    stack_check(av+1, &head);
    if(check_sorted(head) == 0)
        sort_stack(&head);
    for (t_list *i = head; i; i = i->next)
        printf("%d\n", i->number);
    free_stack(&head);
}
