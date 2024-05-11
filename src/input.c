/* input.c - 入力関連の関数 */
#include "input.h"

// XXX: デバッグ用
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

static void	interpret(char *line, t_mgr *mgr)
{
	t_token	*token;
	t_cmd	*cmd;

	token = lexer(line);
	if (token->type == TK_EOF)
	{
		mgr->status = -1;
		free_tokens(token);
		return ;
	}
	print_tokens(token);
	cmd = parser(&token);
	if (cmd == NULL)
	{
		mgr->status = -1;
		free_tokens(token);
		free_cmd(cmd); // 未実装
		return ;
	}
	expand(cmd);        // 未実装
	exec_cmd(cmd, mgr); // 未実装}

	free_tokens(token);
}

int	input(char **envp, t_mgr *mgr)
{
	char	*line;

	rl_outstream = stderr;
	mgr->status = 0;
	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
			break ;
		if (*line)
			add_history(line);
		// TODO: chdirコマンドの処理は、親プロセスで行う必要がある（未実装）
		// ただしpipeコマンド下なら、cdは親プロセスに影響を与えない形で実行される
		interpret(line, mgr);
		free(line);
	}
	exit(mgr->status);
}
