/* free_ast.c - AST（抽象構文木）の解放関数 */
#include "free.h"
#include "minishell.h"

/* free_word - リンクリスト形式のt_word構造体を解放する関数 */
void	free_word(t_word *word)
{
	t_word	*next;

	while (word)
	{
		next = word->next;
		free(word);
		word = next;
	}
}

/* free_redir - linked list形式のt_redir構造体を解放する関数 */
void	free_redir(t_redir *redir)
{
	t_redir	*next;

	while (redir)
	{
		next = redir->next;
		free_word(redir->word_list);
		free(redir);
		redir = next;
	}
}

// ダブルフリー防止用
static void	free_right_cmd(t_cmd *cmd)
{
	t_execcmd	*ecmd;
	t_word		*next;

	ecmd = (t_execcmd *)cmd;
	if (!cmd)
		return ;
	if (cmd->type == EXEC)
	{
		while (ecmd->word_list)
		{
			next = ecmd->word_list->next;
			free(ecmd->word_list);
			ecmd->word_list = next;
		}
		// free(ecmd->word_list->token);
		free_redir(ecmd->redir_list);
	}
	else if (cmd->type == PIPE)
	{
		free_right_cmd(((t_pipecmd *)cmd)->left);
		free_right_cmd(((t_pipecmd *)cmd)->right);
	}
	free(cmd);
}

/* free_cmd - t_cmd構造体およびその派生構造体を解放する関数 */
void	free_cmd(t_cmd *cmd)
{
	t_execcmd	*ecmd;

	ecmd = (t_execcmd *)cmd;
	if (!cmd)
		return ;
	if (cmd->type == EXEC)
	{
		free_word(ecmd->word_list);
		ecmd->word_list = NULL;
		free_redir(((t_execcmd *)cmd)->redir_list);
	}
	else if (cmd->type == PIPE)
	{
		// 左と右両方やると, 二重freeになる箇所あり. たぶんleftのtokenとかword_listで,
		// rightの内容も含んでる部分だと思う. 個別対応必須.
		free_cmd(((t_pipecmd *)cmd)->left);
		// free_cmd(((t_pipecmd *)cmd)->right);
		free_right_cmd(((t_pipecmd *)cmd)->right);
	}
	free(cmd);
}
