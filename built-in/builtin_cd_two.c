/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_two.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 14:28:38 by radib             #+#    #+#             */
/*   Updated: 2026/01/28 21:33:50 by radib            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_pwd(void)
{
	char	*temp;

	temp = get_pwd();
	printf("%s\n", temp);
	free(temp);
}

char	*cd_last(t_env *env)
{
	if (get_value_of_key(env, "OLDPWD") == NULL)
	{
		ft_printf("minishell: cd: OLDPWD not set\n");
		return (NULL);
	}
	return (get_value_of_key(env, "OLDPWD"));
}

int	wich_cd(t_env *env, char *string_after_cd, int x)
{
	char	*temp;

	if (!string_after_cd)
	{
		temp = cd_home(env);
		if (temp)
			if (!chdir(temp))
				return (swap_env(env, "HOME", "PWD", 0));
		return (1);
	}
	else if (ft_strlen(string_after_cd) == 1 && string_after_cd[0] == '-')
	{
		if (cd_last(env))
			x = chdir(cd_last(env));
		if (x == 0)
		{
			temp = get_pwd();
			printf("%s\n", temp);
			swap_env(env, "PWD", temp, 1);
			return (free(temp), 0);
		}
		else
			return (x);
	}
	return (-10);
}

int	call_cd(t_env *env, char *string_after_cd)
{
	int		x;
	char	*temp_pwd;

	temp_pwd = get_pwd();
	x = wich_cd(env, string_after_cd, 1);
	if (x == 0 || x == 1)
	{
		free(temp_pwd);
		return (x);
	}
	if ((chdir(cd_builtin(temp_pwd, string_after_cd, 0)) != 0))
	{
		ft_putstr_fd("minishell: cd:", 2);
		perror(string_after_cd);
		free(temp_pwd);
		return (errno);
	}
	change_value_of_key(env, "OLDPWD", get_value_of_key(env, "PWD"));
	free(temp_pwd);
	temp_pwd = get_pwd();
	change_value_of_key(env, "PWD", temp_pwd);
	free(temp_pwd);
	return (0);
}
