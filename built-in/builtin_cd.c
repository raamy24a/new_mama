/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 03:04:19 by radib             #+#    #+#             */
/*   Updated: 2026/02/03 09:49:08 by radib            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

char	*remove_last(char *string, int *bad)
{
	int	x;

	x = 0;
	if (!is_dir(string))
	{
		*bad = 1;
		return (NULL);
	}
	while (string[x])
		x++;
	while (string[x] != '/')
	{
		string[x] = '\0';
		x--;
	}
	string[x] = '\0';
	if (!is_dir(string))
	{
		*bad = 1;
		return (NULL);
	}
	return (string);
}

char	*add_word(char *string, char *word_to_add)
{
	int	x;
	int	i;

	i = 0;
	x = 0;
	while (string[x])
		x++;
	string[x] = '/';
	x++;
	while (word_to_add[i])
	{
		string[x + i] = word_to_add[i];
		i++;
	}
	string[x + i] = '\0';
	return (string);
}

char	*cd_builtin(char *path, char *string_after_cd, int x, int *bad)
{
	char	**split;

	if (string_after_cd[0] == '/')
		return (string_after_cd);
	split = ft_split(string_after_cd, "/");
	if (split[0][0] == '\0')
		return (string_after_cd);
	while (split[x])
	{
		if ((split[x][0] == '.' && split[x][1] == '.') && split[x][2] == '\0')
		{
			remove_last(path, bad);
			if (*bad)
				return (free_split(split), "/");
		}
		else if (ft_strcmp(split[x], "."))
		{
			add_word(path, split[x]);
			if (*bad)
				return (free_split(split), "/");
		}
		x++;
	}
	return (free_split(split), path);
}

int	swap_env(t_env *env, char *a, char *b, int mode)
{
	char	*c;

	c = ft_strdup(get_value_of_key(env, a));
	if (mode == 1)
	{
		change_value_of_key(env, "PWD", b);
		change_value_of_key(env, "OLDPWD", c);
	}
	else
	{
		change_value_of_key(env, a, get_value_of_key(env, b));
		change_value_of_key(env, b, c);
	}
	free(c);
	return (0);
}

char	*cd_home(t_env *env)
{
	if (!get_value_of_key(env, "HOME"))
	{
		printf("minishell: cd: HOME not set\n");
		return (NULL);
	}
	return (get_value_of_key(env, "HOME"));
}
