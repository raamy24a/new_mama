/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_helper.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 21:20:02 by radib             #+#    #+#             */
/*   Updated: 2026/02/03 23:40:36 by radib            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	error_print_cd(t_env *env)
{
	char	*temp;

	ft_putendl_fd("cd : invalid path going to root", STDERR_FILENO);
	chdir("/");
	temp = ft_strdup(get_value_of_key(env, "PWD"));
	change_value_of_key(env, "PWD", "/");
	change_value_of_key(env, "OLDPWD", ft_strdup(temp));
	free(temp);
	return (-1);
}
