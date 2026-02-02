/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 09:29:59 by radib             #+#    #+#             */
/*   Updated: 2026/02/02 14:50:48 by radib            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	heredoc_helper(char **delimiter)
{
	char	*old;

	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handler_heredoc);
	old = ft_strdup(*delimiter);
	*delimiter = strip_quotes(*delimiter);
	if (strcmp(old, *delimiter) == 0)
		return (free(old), 1);
	else
		return (free(old), 0);
}
