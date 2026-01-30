/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_manipulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 07:23:43 by radib             #+#    #+#             */
/*   Updated: 2026/01/30 14:28:23 by radib            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env	*ft_lstlast_env(t_env *lst)
{
	size_t	i;
	t_env	*last;

	i = 0;
	last = lst;
	if (last == NULL)
		return (NULL);
	while (last -> next)
	{
		last = last -> next;
		i++;
	}
	return (last);
}

char	*get_value_of_key(t_env *env, char *key)
{
	t_env	*tmp;

	tmp = env;
	while (tmp->next && ft_strcmp(key, tmp->key) != 0)
	{
		tmp = tmp->next;
	}
	if (ft_strcmp(key, tmp->key) != 0)
		return (NULL);
	return (tmp->value);
}

void	ft_lstadd_back_env(t_env *lst, t_env *new)
{
	t_env	*temp;

	if (!lst || !new)
		return ;
	temp = ft_lstlast_env(lst);
	if (!temp)
	{
		lst = new;
		return ;
	}
	temp -> next = new;
}

void	end_env_to_char_array(t_env *temp, char **array_env, char *first_part,
			int i)
{
	if (temp->value)
	{
		first_part = ft_strjoin(temp->key, "=");
		array_env[i] = ft_strjoin (first_part, temp->value);
		free(first_part);
	}
	else
		array_env[i] = ft_strjoin(temp->key, "=");
	array_env[i + 1] = NULL;
}

char	**env_to_char_array(t_env *env, int i)
{
	t_env	*temp;
	char	**array_env;
	char	*first_part;

	i = env_len(env);
	array_env = malloc (sizeof (char *) * (i + 1));
	temp = env;
	i = 0;
	while (temp->next)
	{
		if (temp->value)
		{
			first_part = ft_strjoin(temp->key, "=");
			array_env[i] = ft_strjoin (first_part, temp->value);
			free(first_part);
		}
		else
			array_env[i] = ft_strjoin(temp->key, "=");
		temp = temp->next;
		i++;
	}
	end_env_to_char_array(temp, array_env, first_part, i);
	return (array_env);
}
