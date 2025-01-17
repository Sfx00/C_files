#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>

typedef struct sur
{
    int         content;
    struct sur  *next;
}   t_list;


/*----------split---------------*/
static void	*free_word(char **s)
{
	int	i;

	i = 0;
    if (!s)
        return (NULL);
	while (s[i])
	{
		free(s[i]);
		i++;
	}
	free(s);
	return (NULL);
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
	p = (char **)malloc((counter(s, c) + 1) * sizeof(char *));
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
				return (free_word(p));
			arr[1]++;
		}
		while (s[arr[0]] != c && s[arr[0]] != '\0')
			arr[0]++;
	}
	return (p);
}
/*------------------------------*/


/*------list tools-----------*/
t_list	*ft_lstnew(long content)
{
	t_list	*p;

	p = (t_list *)malloc(sizeof(t_list));
	if (!p)
		return (NULL);
	p->content = content;
	p->next = NULL;
	return (p);
}
t_list	*ft_lstlast(t_list *lst, long nb, int *flag)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
    {
        if (lst->content == nb)
            *flag = 0;
		lst = lst->next;
    }
    if (lst->content == nb)
        *flag = 0;
	return (lst);
}
void	ft_lstadd_back(t_list **lst, long nb, int *flag)
{
	t_list	*last;
    t_list *new;

	new = ft_lstnew(nb);
    last = ft_lstlast(*lst, nb, flag);
	if (*lst)
		last->next = new;
	else
		*lst = new;
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
/*-------------------------*/

long	ft_atol(const char *nptr)
{
	long	r;
	int		i;
	int		d;

	r = 0;
	i = 0;
	d = 0;
	while (nptr[i] && (nptr[i] == ' ' || (nptr[i] >= '\t' && nptr[i] <= '\r')))
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			d = 1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9' && nptr[i])
	{
		r = r * 10 + (nptr[i] - '0');
		i++;
	}
	if (d == 1)
		return (-r);
	return (r);
}

void error(t_list **stack, char **buffer)
{
    free_word(buffer);
    free_stack(stack);
    write(2, "Error\n", 6);
    exit(1);
}

int check_input(char *str, long *nb)
{
    int i;

    i = 0;
    if (str[i] == '-' || str[i] == '+')
        i++;

    while(str[i] && (str[i] >= '0' && str[i] <= '9'))
        i++;
    if (str[i] != '\0') // the caracter not digit
        return (0);

    *nb = ft_atol(str);
    if(*nb > INT_MAX || *nb < INT_MIN)
        return(0);

    return (1);
}

# include <stdio.h>
void stock_input(t_list **stack, char **av)
{
    *stack = NULL;
    long nb;
    while(*av)
    {
        char **buffer = ft_split(*av,' ');
        // printf ("av = %s\n", *av);
        // printf ("buffer = %s\n", *buffer);
        if(!*buffer)
            error(stack,buffer);
        int flag = 1;
        int i = 0;
        while(buffer[i] && check_input(buffer[i], &nb) == 1 && flag == 1) //check_input nbr (digit, range of int)
        {
            ft_lstadd_back(stack, nb, &flag); // if flag == 0 there is duplicate in the stack
            i++;
        }
        if (buffer[i] != NULL || flag == 0)
            error(stack,buffer);
        av++;
    }
}


int main(int ac, char* av[])
{
    t_list  *a;

    if(ac == 1)
        return (0);
    stock_input(&a, av + 1);
    for (t_list *i = a; i; i = i->next)// i != NULL
        printf("%d\n", i->content);
    //sort
    //free_list
    free_stack(&a);
}
