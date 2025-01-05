#include "pipex.h"

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
void second_child(_struct *card,char **av,char **env)
{
    char **cmd;
    char *path;
    
    card->outfile = open(av[4], O_CREAT | O_WRONLY | O_TRUNC, 0777);
    if(card->outfile == -1)
        print_error("Faild to open output file");
    if(dup2(card->outfile, 1) == -1)
        print_error("Failed to duplicate file descriptor");
    if(dup2(card->pipe[0],0) == -1)
        print_error("Failed to duplicate file descriptor");
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
        print_error("Failed to duplicate file descriptor");
    if(dup2(card->pipe[1],1) == -1)
        print_error("Failed to duplicate file descriptor");
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
