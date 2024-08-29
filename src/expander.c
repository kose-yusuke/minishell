/* expand.c - 展開処理 */
#include "error.h"
#include "expander.h"

static void	expand_word_list(t_word *word_list, t_mgr *mgr)
{
	t_word	*word;
	t_word	*next_word;

	word = word_list;
	while (word)
	{
		next_word = word->next;
		if (word->token->type == TK_WORD || word->token->type == TK_DQUOTE)
		{
			expand_word_str(&(word->token->word), mgr);
		}
		split_word_token(word);
		word = next_word;
	}
	merge_words(word_list);
}

static bool	is_quoted_heredoc(t_word *word_list)
{
	t_word	*current_word;

	current_word = word_list;
	while (current_word)
	{
		if (current_word->token->type == TK_SQUOTE
			|| current_word->token->type == TK_DQUOTE)
			return (true);
		current_word = current_word->next;
	}
	return (false);
}

static void	merge_heredoc_delimi(t_word *word_list)
{
	bool	has_quote;

	has_quote = is_quoted_heredoc(word_list);
	merge_words(word_list);
	if (has_quote)
	{
		word_list->token->type = TK_SQUOTE;
	}
}

static void	expand_redir_list(t_redir *redir_list, t_mgr *mgr)
{
	while (redir_list)
	{
		if (redir_list->redir_type == TK_HEREDOC)
		{
			merge_heredoc_delimi(redir_list->word_list);
		}
		else
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
