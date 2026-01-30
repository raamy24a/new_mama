/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_manipulation_two.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 09:58:40 by radib             #+#    #+#             */
/*   Updated: 2026/01/30 14:25:52 by radib            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	change_value_of_key(t_env *env, char *key, char *new_value)
{
	t_env	*tmp;

	tmp = env;
	while (tmp->next && ft_strcmp(key, tmp->key) != 0)
	{
		tmp = tmp->next;
	}
	if (ft_strcmp(key, tmp->key) != 0)
		return (0);
	if (!ft_strlen(new_value))
	{
		tmp->status = 2;
		free(tmp->value);
		tmp->value = NULL;
	}
	if (tmp->status == 1)
		free (tmp->value);
	tmp->value = ft_strdup(new_value);
	tmp->status = 1;
	return (1);
}

int	env_len(t_env *env)
{
	t_env	*temp;
	int		i;

	i = 1;
	temp = env;
	while (temp->next)
	{
		i++;
		temp = temp->next;
	}
	return (i);
}
