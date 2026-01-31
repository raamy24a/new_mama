/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 14:36:38 by radib             #+#    #+#             */
/*   Updated: 2026/01/31 01:59:59 by radib            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	call_env(t_env *env)
{
	t_env	*temp;

	temp = env;
	while (temp->next)
	{
		if (temp->status == 1)
			printf("%s=%s\n", temp->key, temp->value);
		else if (temp->status == 2)
			printf("%s=\n", temp->key);
		temp = temp->next;
	}
	if (temp->status == 1)
			printf("%s=%s\n", temp->key, temp->value);
	else if (temp->status == 2)
			printf("%s=\n", temp->key);
	return (0);
}
