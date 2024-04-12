#include "lexer.h"
#include "minishell.h"

static void	mgr_init(t_mgr *mgr)
{
	mgr->tok = NULL;
	mgr->ast = NULL;
	mgr->status = 0;
	mgr->syntax_error = false;
}

// TODO: extern char    **environ; を使う方がいいかも
int	main(int argc, char **argv, char **envp)
{
	t_mgr	mgr;

	(void)argv;
	mgr_init(&mgr);
	if (argc == 1)
		ft_readline(envp, &mgr);
	return (0);
}
