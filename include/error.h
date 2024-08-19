/* error.h - エラー処理に関する関数の定義。 */
#ifndef ERROR_H
# define ERROR_H

# include "token.h"

void	memory_error_and_exit(char *func_name);
void	assert_error(const char *msg, char *location); // debug用
void	error_exit(const char *msg, int exit_status);
void	parser_error(t_token *unexpected_token);
void	report_error(char *cmd, char *file, char *info);

#endif
