/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_execution_four.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 12:55:38 by radib             #+#    #+#             */
/*   Updated: 2026/01/30 14:19:26 by radib            ###   ########.fr       */
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

static int	close_exec_fds(int prev_fd, int pipefd[2])
{
	if (pipefd[0] != -1)
		close(pipefd[0]);
	if (pipefd[1] != -1)
		close(pipefd[1]);
	if (prev_fd != -1)
		close(prev_fd);
	return (1);
}

int	execute_commands(t_command *cmd, t_env *env, int count, t_f **tc)
{
	pid_t	last_pid;
	int		pipefd[2];
	int		prev_fd;

	last_pid = -1;
	prev_fd = -1;
	while (cmd)
	{
		pipefd[0] = -1;
		pipefd[1] = -1;
		(*tc)->cmds = cmd;
		if (init_pipefd(cmd, pipefd))
			return (1);
		last_pid = launch_command(tc, prev_fd, pipefd, env);
		if (last_pid <= 0)
			return (-last_pid * close_exec_fds(prev_fd, pipefd));
		parent_update_fds(&prev_fd, pipefd);
		cmd = cmd->next;
		count++;
	}
	if (prev_fd != -1)
		close(prev_fd);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	return (wait_children(last_pid, count));
}
