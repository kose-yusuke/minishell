/* readline.c - 入力関連の関数 */
#include "ft_readline.h"
#include "minishell.h"

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

const char	*token_to_str(t_token_type type)
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

static void	interpret(char *line, t_mgr *mgr)
{
	mgr->token = lexer(line);
	if (!mgr->token)
	{
		mgr->status = -1;
		fprintf(stderr, "lexer error\n");
		return ;
	}
	if (mgr->token->type == TK_PARSE_ERROR || mgr->token->type == TK_EOF)
	{
		fprintf(stderr, "empty command or parse error\n");
		// later to implement syntax error handling in lexer
		return ;
	}
	print_tokens(mgr->token);
	mgr->cmd = parser(mgr->token);
	if (!mgr->cmd || mgr->cmd->type == NONE)
	{
		fprintf(stderr, "parser error\n");
		// later to implement syntax error handling in parser
		return ;
	}
	run_expansion(mgr->cmd, mgr->env_table); // mgrを直接渡すべきでは？
	exec_cmd(mgr->cmd, mgr);
}

static void	reset_resources(t_mgr *mgr)
{
	free_tokens(mgr->token);
	free_cmd(mgr->cmd);
	mgr->token = NULL;
	mgr->cmd = NULL;
}

int	ft_readline(t_mgr *mgr)
{
	char	*line;

	rl_outstream = stderr;
	mgr->status = 0;
	while (mgr->status != -1)
	{
		line = readline("minishell$ ");
		if (!line)
		{
			mgr->status = -1;
			break ;
		}
		if (*line)
			add_history(line);
		// TODO: chdirコマンドの処理は、親プロセスで行う（未実装）
		// ただしpipeのなかでのcdは、親プロセスに影響を与えない
		interpret(line, mgr);
		free(line);
		reset_resources(mgr);
	}
	return (mgr->status);
}
