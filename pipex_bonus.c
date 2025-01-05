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

int open_file(char *file, int flag)
{
    int fd;
    if(flag == 0)
    {
        if(access(file,F_OK) == -1)
            print_error("No such file or directory");
        else
        {
            fd = open(file, O_RDONLY);
            if(fd == -1)
                print_error("Files Can't Be Opened");
            return (fd);
        }
    }
    else
    {
        fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0777);
        if(fd == -1)
                print_error("Files Can't Be Opened");
        return(fd);
    }
}
void free_strings(char **str)
{
    while(*str)
    {
        free(*str);
        *str = NULL;
        str++;
    }
}
void execute(char *argv, char **env)
{
    char *path;
    char **cmd;

    cmd = ft_split(argv,' ');
     if (!cmd)
        print_error("Failed to split command");
    path= find_path(cmd[0],env);
    if(!path)
    {
        free_strings(cmd);
        print_error("Not found PATH of command");
    }
    execve(path,cmd,env);
    free(path);
    free_strings(cmd);
    print_error("Command not found");
}

void pipex(char *cmd, char **env)
{
    int status;
    int pid;
    int pipe_fd[2];

    if(pipe(pipe_fd) == -1)
        print_error("Failed to create pipe");

    pid = fork();
    if(pid < 0)
        print_error("faild to create a child");

   if(pid)
   {
        close(pipe_fd[1]);
        dup2(pipe_fd[0], 0);
        waitpid(pid, &status, 0);
   }
   else
   {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], 1);
        execute(cmd,env);
   }

}

void handle_here_doc(char *limiter)
{
    int     pipe_fd[2];
    char    *line;

    if (pipe(pipe_fd) == -1)
        print_error("Pipe error");
    while (1)
    {
        write(1, "heredoc> ", 9);
        line = get_next_line(0);
        if (!line)
            break;
        if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0 && 
            line[ft_strlen(limiter)] == '\n')
        {
            free(line);
            break;
        }
        write(pipe_fd[1], line, ft_strlen(line));
        free(line);
    }
    close(pipe_fd[1]);
    dup2(pipe_fd[0], 0);
    close(pipe_fd[0]);
}

int main(int ac, char **av,char **env)
{
    int fd[2];
    int i;
    
    if (!env || ac < 5)
        print_error("Error: Invalid arguments");
    if(ac > 5 && (ft_strncmp(av[1], "here_doc",8) == 0))
    {
        if(ac < 6)
            print_error("Usage: ./pipex here_doc LIMITER cmd1 cmd2 file");
        handle_here_doc(av[2]);
        fd[1] = open_file(av[ac - 1], 1);
        i = 3;
    }
    else
    {
        fd[0] = open_file(av[1], 0);
        fd[1] = open_file(av[ac - 1], 1);
        dup2(fd[0], 0);
        i = 2;
    }
    dup2(fd[1],1);
    while(i < ac - 2)
        pipex(av[i++], env);
    execute(av[i],env);
}
