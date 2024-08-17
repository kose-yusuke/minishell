/* parser_init.c - パーサーの初期化関数の定義 */
#include "parser.h"

t_word	*init_word(t_token *token)
{
	t_word	*new_word;

	new_word = calloc(1, sizeof(*new_word));
	if (!new_word)
	{
		report_error("init_word", NULL, "memory allocation failed");
		return (NULL);
	}
	new_word->token = token;
	new_word->next = NULL;
	return (new_word);
}

t_redir	*init_redir(t_token_type type, int fd)
{
	struct s_redir	*new_redir;

	new_redir = calloc(1, sizeof(*new_redir));
	if (!new_redir)
	{
		report_error("init_redir", NULL, "memory allocation failed");
		return (NULL);
	}
	new_redir->redir_type = type;
	new_redir->fd = fd;
	new_redir->backup_fd = -1;
	new_redir->word_list = NULL;
	new_redir->next = NULL;
	return (new_redir);
}

t_cmd	*init_execcmd(void)
{
	struct s_execcmd	*cmd;

	cmd = calloc(1, sizeof(*cmd));
	if (!cmd)
	{
		report_error("init_execcmd", NULL, "memory allocation failed");
		return (NULL);
	}
	cmd->type = EXEC;
	cmd->word_list = NULL;
	cmd->redir_list = NULL;
	return ((t_cmd *)cmd);
}

t_cmd	*init_pipecmd(t_cmd *left, t_cmd *right)
{
	struct s_pipecmd	*cmd;

	// TODO: leftとrightがNULLだったらどうする？
	cmd = calloc(1, sizeof(*cmd));
	if (!cmd)
	{
		report_error("init_pipecmd", NULL, "memory allocation failed");
		free_cmd(left);
		free_cmd(right);
		return (NULL);
	}
	cmd->type = PIPE;
	cmd->left = left;
	cmd->right = right;
	return ((t_cmd *)cmd);
}
