#include "../include/lexer.h"
#include "../include/minishell.h"

char *search_path(const char *filename)
{
    char path[PATH_MAX];
    char *value;
    char *end;
    
    value = getenv("PATH");
    while (*value)
    {
        bzero(path, PATH_MAX);
        end = strchr(value, ':');
        if (end)
			strncpy(path, value, end - value);
		else
			strlcpy(path, value, PATH_MAX);
		strlcat(path, "/", PATH_MAX);
		strlcat(path, filename, PATH_MAX);
        if (access(path, X_OK) == 0)
        {
            char *dup;
            dup = strdup(path);
            if (dup == NULL)
				fatal_error("strdup");
			return (dup);
        }
        if (end == NULL)
			return (NULL);
		value = end + 1;
    }
    return (NULL);
}

void	validate_access(const char *path, const char *filename)
{
	if (path == NULL)
		err_exit(filename, "command not found", 127);
	if (access(path, F_OK) < 0)
		err_exit(filename, "command not found", 127);
}

int exec(char *argv[])
{
    extern char **environ;
    const char *path = argv[0];
	pid_t		pid;
	int			wstatus;

    pid = fork();
    if (pid < 0)
        fatal_error("fork");
    else if (pid == 0)
    {
        if (strchr(path, '/') == NULL)
        {
            path = search_path(path);
        }
		validate_access(path, argv[0]);
		execve(path, argv, environ);
        fatal_error("execve");
    }
    else{
        wait(&wstatus);
        return (WEXITSTATUS(wstatus));
    }
    
}