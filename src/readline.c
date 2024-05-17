/* readline.c - 入力関連の関数 */
#include "minishell.h"
#include "readline.h"
#include "executor.h"

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
// parser: デバッグ用

// ノードのインデントを管理するためのヘルパー関数
static void print_indent(int level)
{
    for (int i = 0; i < level; i++)
    {
        printf("  ");
    }
}

// コマンドの種類を文字列に変換するヘルパー関数
static const char *cmd_type_to_str(t_cmd_type type)
{
    switch (type)
    {
    case NONE:
        return "NONE";
    case EXEC:
        return "EXEC";
    case PIPE:
        return "PIPE";
    default:
        return "UNKNOWN";
    }
}

// 単語リストを表示するヘルパー関数
static void print_word_list(t_word *word_list, int level)
{
    t_word *current = word_list;
    while (current)
    {
        print_indent(level);
        printf("Word: %s\n", current->token->word);
        current = current->next;
    }
}

// リダイレクトリストを表示するヘルパー関数
static void print_redir_list(t_redir *redir_list, int level)
{
    t_redir *current = redir_list;
    while (current)
    {
        print_indent(level);
        printf("Redirection: type=%d, fd=%d\n", current->redir_type, current->fd);
        print_word_list(current->word_list, level + 1);
        current = current->next;
    }
}

void print_asts(t_cmd *cmd, int level)
{
    if (!cmd)
    {
        return;
    }

    print_indent(level);
    printf("Command type: %s\n", cmd_type_to_str(cmd->type));

    if (cmd->type == EXEC)
    {
        t_execcmd *ecmd = (t_execcmd *)cmd;
        print_word_list(ecmd->word_list, level + 1);
        print_redir_list(ecmd->redir_list, level + 1);
    }
    else if (cmd->type == PIPE)
    {
        t_pipecmd *pcmd = (t_pipecmd *)cmd;
        print_indent(level);
        printf("Left:\n");
        print_asts(pcmd->left, level + 1);
        print_indent(level);
        printf("Right:\n");
        print_asts(pcmd->right, level + 1);
    }
}
// parser: デバッグ用終了

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
	print_asts(cmd, 0);
	if (cmd == NULL)
	{
		mgr->status = -1;
		free_tokens(token);
		// free_cmd(cmd); // 未実装
		return ;
	}
	// expand(cmd);        // 未実装
	// exec_cmd(cmd); // 未実装}

	free_tokens(token);
}

int	ft_readline(char **envp, t_mgr *mgr)
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
