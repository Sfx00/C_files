#include "pipex.h"

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