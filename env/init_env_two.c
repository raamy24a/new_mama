/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env_two.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 14:56:06 by radib             #+#    #+#             */
/*   Updated: 2026/01/30 14:11:55 by radib            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	**init_default_env(void)
{
	char	*temp;
	char	*temp_two;
	char	**env;
	char	*pwd;

	pwd = get_pwd();
	temp = ft_strjoin("PWD=", pwd);
	free(pwd);
	temp_two = ft_strjoin (temp, "-SHLVL=0-_=/usr/bin/env-OLDPWD");
	free(temp);
	env = ft_split(temp_two, "-");
	free(temp_two);
	return (env);
}

t_env	*init_env(char **env, int i)
{
	t_env	*head;
	t_env	*node;

	head = NULL;
	while (env[i])
		i++;
	if (i < 10)
		env = init_default_env();
	while (*env)
	{
		node = duplicate_env(*env);
		if (!node)
		{
			free_env(head);
			return (NULL);
		}
		add_to_env(&head, node);
		env++;
	}
	return (head);
}
