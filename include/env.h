/* env.h - 環境変数の操作に関連する関数の定義。環境変数の設定、取得、更新、削除など。 */
#ifndef ENV_H
# define ENV_H

# include "ft_hash.h"
# include "minishell.h"

t_hash_table	*create_env_table(void);

#endif /* ENV_H */
