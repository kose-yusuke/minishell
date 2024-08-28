/* expand.c - 展開処理 */
#include "error.h"
#include "expander.h"

static void	expand_word_token(t_token *word_token, t_mgr *mgr)
{
	char	*current_ptr;

	if (word_token->type != TK_WORD && word_token->type != TK_DQUOTE)
		return ;
	current_ptr = word_token->word;
	while (current_ptr && *current_ptr)
	{
		expand_variable(&(word_token->word), &current_ptr, mgr);
	}
}

static void	expand_word_list(t_word *word_list, t_mgr *mgr)
{
	t_word	*word_to_expand;
	t_word	*next_word;

	word_to_expand = word_list;
	while (word_to_expand)
	{
		next_word = word_to_expand->next;
		expand_word_token(word_to_expand->token, mgr);
		split_word_token(word_to_expand);
		word_to_expand = next_word;
	}
	merge_words(word_list);
}

void	expand_redir_list(t_redir *redir_list, t_mgr *mgr)
{
	while (redir_list)
	{
		// heredocのデリミタは展開しないが、mergeだけは必要になる
		if (redir_list->redir_type != TK_HEREDOC)
		{
			expand_word_list(redir_list->word_list, mgr);
		}
		redir_list = redir_list->next;
	}
}

void	run_expansion(t_cmd *cmd, t_mgr *mgr)
{
	t_execcmd	*ecmd;
	t_pipecmd	*pcmd;

	if (!cmd || !mgr || !mgr->env_table)
		return ;
	else if (cmd->type == EXEC)
	{
		ecmd = (t_execcmd *)cmd;
		expand_word_list(ecmd->word_list, mgr);
		expand_redir_list(ecmd->redir_list, mgr);
		mgr->exit_status = SC_SUCCESS;
	}
	else if (cmd->type == PIPE)
	{
		pcmd = (t_pipecmd *)cmd;
		run_expansion(pcmd->left, mgr);
		run_expansion(pcmd->right, mgr);
	}
}
