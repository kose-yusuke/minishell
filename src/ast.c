/* ast.c - AST（抽象構文木）に関する関数の実装 */
#include "ast.h"
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

/* free_cmd - t_cmd構造体およびその派生構造体を解放する関数 */
void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->type == EXEC)
	{
		free_word(((t_execcmd *)cmd)->word_list);
		free_redir(((t_execcmd *)cmd)->redir_list);
	}
	else if (cmd->type == PIPE)
	{
		free_cmd(((t_pipecmd *)cmd)->left);
		free_cmd(((t_pipecmd *)cmd)->right);
	}
	free(cmd);

}
