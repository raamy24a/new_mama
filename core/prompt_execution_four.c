/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_execution_four.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 12:55:38 by radib             #+#    #+#             */
/*   Updated: 2026/01/30 13:04:25 by radib            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	exec_exit_pipe(char **command)
{
	t_long_verif	*nbr;
	long long		exit_nbr;

	if (!command[1])
		return (0);
	nbr = ft_verif_atoll(command[1], 1, 0, 0);
	exit_nbr = nbr->nbr;
	if (nbr->status == 0)
	{
		printf("minishell: exit: %s: numeric argument required\n", command[1]);
		free(nbr);
		return (2);
	}
	else if (command[2])
	{
		printf ("minishell: exit: too many arguments\n");
		printf("exit\n");
		free(nbr);
		return (1);
	}
	free(nbr);
	return (exit_nbr % 256);
}