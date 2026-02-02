/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_two.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 14:14:08 by radib             #+#    #+#             */
/*   Updated: 2026/02/02 10:03:59 by radib            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	expand_str_two(char **str, char **argv, int i)
{
	char	*copy;
	char	*temp;
	int		x;

	if (!str)
		return (1);
	copy = ft_strdup(*argv);
	copy[i] = '\0';
	temp = ft_strjoin(copy, str[1]);
	free(*argv);
	(*argv) = ft_strjoin(temp, str[0]);
	free(copy);
	free(temp);
	if (!*argv)
		return (-1);
	x = ft_strlen(str[1]);
	if (!x)
		x++;
	free_split(str);
	return (x - 1);
}

int	skip_single(char *argv, int i)
{
	if (argv[i] == 39)
	{
		while (argv[++i] != 39)
			;
	}
	return (i);
}

void	expand_argv(char **argv, int i, int last_status, t_env *env)
{
	int	last;
	int	yes;

	yes = 1;
	while (argv[0][i])
	{
		last = i;
		if (argv[0][i] == '"')
			yes *= -1;
		if (yes == 1)
			i = skip_single(argv[0], i);
		if (argv[0][i] == '$')
		{
			if (argv[0][i + 1] && argv[0][i + 1] == '?')
				i += expand_str(last_status, argv, i);
			else
				i += expand_str_two(expand_dollars(&argv[0][i + 1], 0, env),
						argv, i);
		}
		if (i < last)
			return ;
		if (argv[0][i] && argv[0][i] != '$')
			i++;
	}
	return ;
}

void	expand_commands(t_command *cmds, t_env *env, int last_status)
{
	int			i;
	char		*old;

	while (cmds)
	{
		i = -1;
		while (cmds->argv[++i])
		{
			expand_argv(&cmds->argv[i], 0, last_status, env);
			old = cmds->argv[i];
			cmds->argv[i] = strip_quotes(cmds->argv[i]);
			if (cmds->argv[i] != old)
				free(old);
		}
		cmds = cmds->next;
	}
}
