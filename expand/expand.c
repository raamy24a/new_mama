/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 00:20:02 by radib             #+#    #+#             */
/*   Updated: 2026/01/31 00:25:18 by radib            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static size_t	stripped_length(const char *token)
{
	size_t	i;
	size_t	len;
	char	quote;

	i = 0;
	len = 0;
	quote = 0;
	while (token[i])
	{
		if (token[i] == 39 || token[i] == '"')
		{
			quote = token[i++];
			while (token[i] && token[i++] != quote)
				len++;
			continue ;
		}
		len++;
		i++;
	}
	return (len);
}

char	*strip_quotes(const char *token)
{
	char	*result;
	size_t	i;
	size_t	j;
	char	quote;

	result = (char *)ft_calloc(stripped_length(token) + 1, sizeof(char));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	quote = 0;
	while (token[i])
	{
		if (token[i] == 39 || token[i] == '"')
		{
			quote = token[i++];
			while (token[i] && token[i] != quote)
				result[j++] = token[i++];
			if (token[i] == quote)
				i++;
		}
		else
			result[j++] = token[i++];
	}
	return (result);
}

char	**expand_dollars(char *s, int x, t_env *env)
{
	char	*temp;
	char	**return_str;

	if (ft_strlen(s) == 1 || !ft_strlen(s))
		return (NULL);
	return_str = malloc(sizeof(char *) * 3);
	while (s[x] >= '0' && (s[x] <= '9' || s[x] >= 'A')
		&& (s[x] <= 'Z' || s[x] >= 'a') && (s[x] <= 'z' || s[x] == '_'))
		x++;
	temp = malloc(sizeof(char) * (x + 1));
	if (!temp)
		exit_call(1, env, NULL);
	x = 0;
	while (s[x] >= '0' && (s[x] <= '9' || s[x] >= 'A')
		&& (s[x] <= 'Z' || s[x] >= 'a') && (s[x] <= 'z' || s[x] == '_'))
	{
		temp[x] = s[x];
		x++;
	}
	temp[x] = '\0';
	return_str[1] = ft_strdup(get_value_of_key(env, temp));
	return_str[2] = NULL;
	return_str[0] = ft_strdup(&s[x]);
	return (free(temp), return_str);
}

int	expand_str(int nbr, char **argv, int i)
{
	char	*copy;
	char	*str;
	char	*temp;
	int		x;

	str = ft_itoa(nbr);
	if (!str)
		return (-1);
	x = ft_strlen(str);
	if (!x)
		x++;
	copy = ft_strdup(*argv);
	copy[i] = '\0';
	temp = ft_strjoin(copy, str);
	free(*argv);
	*argv = ft_strjoin(temp, &copy[i + 2]);
	free(temp);
	free(str);
	free(copy);
	if (!*argv)
		return (-1);
	return (x - 1);
}
