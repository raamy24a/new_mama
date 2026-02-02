/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_execution_four.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/30 12:55:38 by radib             #+#    #+#             */
/*   Updated: 2026/02/02 08:53:33 by radib            ###   ########.fr       */
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

void	child_execute_suite(t_f **tc, int input_fd, int output_fd, t_env *env)
{
	g_last_status = 0;
	if (apply_redirections((*tc)->cmds->redirs, &input_fd, &output_fd) == -1)
		exit_call(EXIT_FAILURE, env, (*tc));
	if (dup2(input_fd, STDIN_FILENO) == -1)
		perror("dup2");
	if (dup2(output_fd, STDOUT_FILENO) == -1)
		perror("dup2");
	if (input_fd != STDIN_FILENO)
		close(input_fd);
	if (output_fd != STDOUT_FILENO)
		close(output_fd);
	if (g_last_status == 130)
		exit_call_silent (130, env, *tc);
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

void	apply_redirection_helper(t_f **tc, int pipefd[2],
		int prev_fd, t_env *env)
{
	int	in_fd;
	int	out_fd;

	signal(SIGINT, handler_heredoc);
	signal(SIGQUIT, SIG_IGN);
	in_fd = prev_fd;
	out_fd = pipefd[1];
	if (in_fd == -1)
		in_fd = STDIN_FILENO;
	if (out_fd == -1)
		out_fd = STDOUT_FILENO;
	if (apply_redirections((*tc)->cmds->redirs, &in_fd, &out_fd) == -1)
		exit_call_silent (1, env, *tc);
	if (in_fd != STDIN_FILENO && in_fd != prev_fd)
		close(in_fd);
	if (out_fd != STDOUT_FILENO && out_fd != pipefd[1])
		close(out_fd);
}
