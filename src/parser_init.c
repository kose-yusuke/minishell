/* parser_init.c - パーサーの初期化関数の定義 */
#include "free.h"
#include "parser.h"
#include "utils.h"
#include "xlibc.h"

t_word	*init_word(t_token *token)
{
	t_word	*new_word;

	new_word = xmalloc(sizeof(*new_word));
	ft_bzero(new_word, sizeof(*new_word));
	new_word->token = token;
	new_word->next = NULL;
	return (new_word);
}

t_redir	*init_redir(t_token_type type, int fd)
{
	t_redir	*new_redir;

	new_redir = xmalloc(sizeof(*new_redir));
	ft_bzero(new_redir, sizeof(*new_redir));
	new_redir->redir_type = type;
	new_redir->fd = fd;
	new_redir->word_list = NULL;
	new_redir->next = NULL;
	return (new_redir);
}

t_cmd	*init_execcmd(void)
{
	t_execcmd	*cmd;

	cmd = xmalloc(sizeof(*cmd));
	ft_bzero(cmd, sizeof(*cmd));
	cmd->type = EXEC;
	cmd->word_list = NULL;
	cmd->redir_list = NULL;
	return ((t_cmd *)cmd);
}

t_cmd	*init_pipecmd(t_cmd *left, t_cmd *right)
{
	t_pipecmd	*cmd;

	if (!left || !right)
	{
		free_cmd(left);
		free_cmd(right);
		return (NULL);
	}
	cmd = xmalloc(sizeof(*cmd));
	ft_bzero(cmd, sizeof(*cmd));
	cmd->type = PIPE;
	cmd->left = left;
	cmd->right = right;
	return ((t_cmd *)cmd);
}
