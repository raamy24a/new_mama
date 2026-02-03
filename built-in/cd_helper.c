/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_helper.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 21:20:02 by radib             #+#    #+#             */
/*   Updated: 2026/02/03 23:43:42 by radib            ###   ########.fr       */
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
	change_value_of_key(env, "OLDPWD", temp);
	free(temp);
	return (-1);
}
