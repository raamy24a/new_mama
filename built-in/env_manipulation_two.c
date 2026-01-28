/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_manipulation_two.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 09:58:40 by radib             #+#    #+#             */
/*   Updated: 2026/01/28 21:15:52 by radib            ###   ########.fr       */
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
