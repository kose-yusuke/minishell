/* readline.c - 入力関連の関数 */
#include "ft_readline.h"
#include "minishell.h"

// XXX: デバッグ用
static const char	*token_to_str(t_token_type type)
{
	switch (type)
	{
	case TK_UNDEF_TOKEN:
		return ("TK_UNDEF_TOKEN");
	case TK_PARSE_ERROR:
		return ("TK_PARSE_ERROR");
	case TK_EOF:
		return ("TK_EOF");
	case TK_WORD:
		return ("TK_WORD");
	case TK_SQUOTE:
		return ("TK_SQUOTE");
	case TK_DQUOTE:
		return ("TK_DQUOTE");
	case TK_IO_NUM:
		return ("TK_IO_NUM");
	case TK_PIPE:
		return ("TK_PIPE");
	case TK_REDIR_IN:
		return ("TK_REDIR_IN");
	case TK_REDIR_OUT:
		return ("TK_REDIR_OUT");
	case TK_HEREDOC:
		return ("TK_HEREDOC");
	case TK_APPEND:
		return ("TK_APPEND");
	case TK_BLANK:
		return ("TK_BLANK");
	case TK_NL:
		return ("TK_NL");
	default:
		return ("UNKNOWN_TYPE");
	}
}

// XXX: デバッグ用
static void	print_tokens(t_token *token)
{
	t_token	*current;
	size_t	i;

	current = token;
	i = 0;
	while (current)
	{
		printf("--------------------------------\n");
		printf("Token [%zu]\n", i++);
		printf("Type: %s\n", token_to_str(current->type));
		if (current->word)
		{
			printf("Word: %s\n", current->word);
		}
		current = current->next;
	}
}

static void	reset_resources(t_mgr *mgr)
{
	if (mgr->status == 0 && mgr->syntax_error) // これなんだっけ？
		mgr->status = 1;
	free_tokens(mgr->token);
	free_cmd(mgr->cmd);
	mgr->token = NULL;
	mgr->cmd = NULL;
}

static void	interpret(char *line, t_mgr *mgr)
{
	mgr->token = lexer(line);
	if (!mgr->token || mgr->token->type == TK_PARSE_ERROR)
	{
		report_error("lexer error", 0, 0); // ?
		return ;
	}
	if (mgr->token->type == TK_EOF)
	{
		return ;
	}
	print_tokens(mgr->token); // debug
	mgr->cmd = parser(mgr->token);
	if (!mgr->cmd || mgr->cmd->type == NONE)
	{
		report_error("parser error", 0, 0); // ?
		return ;
	}
	run_expansion(mgr->cmd, mgr);
	exec_cmd(mgr->cmd, mgr);
}

void	ft_readline(t_mgr *mgr)
{
	char	*line;

	rl_outstream = stderr;
	mgr->status = 0; // ?
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			free_mgr_resources(mgr);
			error_exit("failed to read line", EXIT_FAILURE); // ?
		}
		if (*line)
		{
			add_history(line);
			interpret(line, mgr);
		}
		free(line);
		reset_resources(mgr);
	}
}
