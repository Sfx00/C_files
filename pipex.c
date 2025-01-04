#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>
#include "libft.h"

typedef struct data
{
    int infile;
    int outfile;
    int pipe[2];
    pid_t pid1;
    pid_t pid2;
    char **cmd1;
    char **cmd2;

}_struct;


void print_error(char *str)
{
    while(*str)
    {
        write(2,str,1);
        str++;
    }
    write(2,"\n",1);
    exit(1);
}

char *check_path(char **full_path,char *cmd)
{
    int i;

    i = 0;
    while(full_path[i])
    {
        char *path;
        char *tmp;

        tmp = ft_strjoin(full_path[i],"/");
        if(!tmp)
            return(NULL);
        path = ft_strjoin(tmp,cmd);
        free(tmp);
        if(!path)
            return(NULL);
        if(access(path,X_OK) == 0)
            return (path);
        free(path);
        i++;
    }
    return(NULL);
}

char *get_path(char **env,char *cmd)
{
    int i;
    char **full_path;
    char *path;
    i = 0;
    while(env[i])
    {
        if(ft_strncmp(env[i],"PATH=",5) == 0)
        {
            full_path = ft_split(env[i] + 5 ,':');
            if(!full_path)
                return(NULL);
            path = check_path(full_path,cmd);
            if(!path)
                return (NULL);
            else
                return (path);
        }
        i++;
    }
    return(NULL);
}

char *find_path(char *cmd,char **env)
{
    char *path;

    if(ft_strchr(cmd,'/'))
    {
        if(access(cmd,X_OK) == 0)
            return (cmd);
        return(NULL);
    }

    path = get_path(env,cmd);
    if(!path)
        return (NULL);

    return (path);
}
void second_child(_struct *card,char **av,char **env)
{
    char **cmd;
    char *path;
    
    card->outfile = open(av[4], O_CREAT | O_WRONLY | O_TRUNC, 0777);
    if(card->outfile == -1)
        print_error("Faild to open output file");
    if(dup2(card->outfile, 1) == -1)
        print_error("Faild to dup2 child2");
    if(dup2(card->pipe[0],0) == -1)
        print_error("Faild to dup2 child2");
    close(card->infile);
    close(card->pipe[1]);
    cmd = ft_split(av[3],' ');
    if(!cmd)
        print_error("Faild to split command");
    path= find_path(cmd[0],env);
    if(!path)
    {
        free(cmd);
        print_error("Not found PATH of command");
    }
    execve(path,cmd,env);
    free(path);
    free(cmd);
    print_error("Command not found");
}

void first_child(_struct *card,char **av,char **env)
{
    char **cmd;
    char *path;

    card->infile = open(av[1],O_RDONLY);
    if(card->infile == -1)
        print_error("Failed to open input file");
    if(dup2(card->infile,0) == -1)
        print_error("Failed to dup2 in first child");
    if(dup2(card->pipe[1],1) == -1)
        print_error("Faild to dup2 child1");
    close(card->pipe[0]);
    close(card->outfile);
    cmd = ft_split(av[2],' ');
     if (!cmd)
        print_error("Failed to split command");
    path= find_path(cmd[0],env);
    if(!path)
    {
        free(cmd);
        print_error("Not found PATH of command");
    }
    execve(path,cmd,env);
    free(path);
    free(cmd);
    print_error("Command not found");
}


void pipex(_struct *card,char **av,char **env)
{
    int status;
    if(pipe(card->pipe) == -1)
        print_error("Failed to create pipe");

    card->pid1 = fork();
    if(card->pid1 < 0)
        print_error("faild to create a child");

    if(card->pid1 == 0)
        first_child(card,av,env);

    card->pid2 = fork();
    if(card->pid2 < 0)
        print_error("faild to create a child");

    if(card->pid2 == 0)
        second_child(card,av,env);
    
    close(card->pipe[0]);
    close(card->pipe[1]);

    waitpid(card->pid1,&status,0);
    waitpid(card->pid2,&status,0);
}



int main(int ac, char **av, char **env)
{
     _struct card;

    if(ac != 5)
        print_error("Usage: ./pipex file1 cmd1 cmd2 file2");
    if(!env)
        print_error("Error empty Environment");
    
    pipex(&card,av,env);
    
}
