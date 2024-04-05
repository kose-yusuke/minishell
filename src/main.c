#include "../include/lexer.h"
#include "../include/minishell.h"

void mgr_init(t_mgr *mgr)
{
    mgr->tok = NULL;
	mgr->ast = NULL;
	mgr->status = 0;
	mgr->syntax_error = false;
}

int main(int argc, char **argv, char **envp)
{
    t_mgr mgr;

    mgr_init(&mgr);
    if (argc == 1)
        ft_readline(envp);
}