/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_execution_three.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 14:35:30 by radib             #+#    #+#             */
/*   Updated: 2026/01/29 18:27:29 by radib            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_builtin_child(char *builtin_str)
{
	if (ft_strcmp(builtin_str, "echo") == 0)
		return (1);
	else if (ft_strcmp(builtin_str, "pwd") == 0)
		return (2);
	else if (ft_strcmp(builtin_str, "export") == 0)
		return (3);
	else if (ft_strcmp(builtin_str, "unset") == 0)
		return (4);
	else if (ft_strcmp(builtin_str, "cd") == 0)
		return (5);
	else if (ft_strcmp(builtin_str, "exit") == 0)
		return (6);
	else if (ft_strcmp(builtin_str, "env") == 0)
		return (7);
	return (0);
}

void	child_execute_suite(t_f **tc, int input_fd, int output_fd, t_env *env)
{
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
}

int	child_execute(t_f **tc, int prev_fd, int *pipefd, t_env *env)
{
	int		input_fd;
	int		output_fd;
	char	**str_env;

	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	input_fd = prev_fd;
	output_fd = pipefd[1];
	if (input_fd == -1)
		input_fd = STDIN_FILENO;
	if (output_fd == -1)
		output_fd = STDOUT_FILENO;
	child_execute_suite(tc, input_fd, output_fd, env);
	if (prev_fd != -1 && prev_fd != STDIN_FILENO)
		close(prev_fd);
	if (pipefd[1] != -1 && pipefd[1] != STDOUT_FILENO)
		close(pipefd[1]);
	if (pipefd[0] != -1)
		close(pipefd[0]);
	if (is_builtin_child((*tc)->cmds->argv[0]))
		exit_call_silent(exec_builtin(is_builtin_child((*tc)->cmds->argv[0]),
				(*tc)->cmds->argv, env), env, *tc);
	str_env = env_to_char_array(env, 0);
	return (px_exec((*tc)->cmds->argv, str_env, env, *tc));
}

static int	init_pipefd(t_command *cmd, int pipefd[2])
{
	pipefd[0] = -1;
	pipefd[1] = -1;
	if (cmd && cmd->pipe_after)
	{
		if (pipe(pipefd) == -1)
			return (perror("pipe"), 1);
	}
	return (0);
}

static int	parent_update_fds(int *prev_fd, int pipefd[2])
{
	if (pipefd[1] != -1)
		close(pipefd[1]);
	if (*prev_fd != -1)
		close(*prev_fd);
	*prev_fd = pipefd[0];
	return (0);
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
