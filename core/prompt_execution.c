/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_execution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 15:32:14 by acollon           #+#    #+#             */
/*   Updated: 2026/02/02 10:03:07 by radib            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <string.h>

int	handle_heredoc(char *delimiter, t_env *env)
{
	int		pipefd[2];
	char	*line;
	int		to_expand;

	to_expand = heredoc_helper(&delimiter);
	if (pipe(pipefd) == -1)
		return (perror("pipe"), -1);
	while (1)
	{
		line = readline("> ");
		if (!line || !ft_strcmp(line, delimiter))
		{
			if (!line && g_last_status != 130)
				printf("minishell: end-of-file (wanted `%s')\n", delimiter);
			break ;
		}
		if (to_expand)
			expand_argv(&line, 0, g_last_status, env);
		ft_putendl_fd(line, pipefd[1]);
		free(line);
	}
	close(pipefd[1]);
	return (free(line), free(delimiter), pipefd[0]);
}

int	fd_value(int fd, t_redir *redir, t_env *env)
{
	if (redir->type == REDIR_IN)
		fd = open(redir->target, O_RDONLY);
	else if (redir->type == REDIR_OUT)
		fd = open(redir->target, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	else if (redir->type == REDIR_APPEND)
		fd = open(redir->target, O_CREAT | O_APPEND | O_WRONLY, 0644);
	else if (redir->type == REDIR_HEREDOC)
		fd = handle_heredoc(redir->target, env);
	return (fd);
}

int	apply_redirections(t_redir *redir, int *input_fd,
		int *output_fd, t_env *env)
{
	int	fd;

	while (redir)
	{
		fd = -1;
		fd = fd_value(fd, redir, env);
		if (fd < 0)
			return (perror(redir->target), -1);
		if (redir->type == REDIR_IN || redir->type == REDIR_HEREDOC)
		{
			if (*input_fd != STDIN_FILENO && *input_fd != -1)
				close(*input_fd);
			*input_fd = fd;
		}
		else
		{
			if (*output_fd != STDOUT_FILENO && *output_fd != -1)
				close(*output_fd);
			*output_fd = fd;
		}
		redir = redir->next;
	}
	return (0);
}

void	exec_exit(char **command, t_env *env, t_f *tc)
{
	t_long_verif	*nbr;
	long long		exit_nbr;

	if (!command[1])
		exit_call(0, env, tc);
	nbr = ft_verif_atoll(command[1], 1, 0, 0);
	exit_nbr = nbr->nbr;
	if (nbr->status == 0)
	{
		printf("minishell: exit: %s: numeric argument required\n", command[1]);
		free(nbr);
		exit_call(2, env, tc);
	}
	else if (command[2])
	{
		printf ("minishell: exit: too many arguments\n");
		printf("exit\n");
		free(nbr);
		return ;
	}
	free(nbr);
	exit_call(exit_nbr, env, tc);
}

int	exec_builtin(int x, char **command, t_env *env, t_f *tc)
{
	t_env	*temp;
	int		return_value;

	temp = NULL;
	return_value = 0;
	if (x == 1)
		return_value = echobuiltin(&command[1], 1, 0);
	else if (x == 2)
		return_value = call_pwd();
	else if (x == 3)
		return_value = export_builtin(env, command, 1);
	else if (x == 4)
		return_value = builtin_unset(env, command, temp, 1);
	else if (x == 5)
		return_value = call_cd(env, command[1]);
	else if (x == 8)
		return_value = exec_exit_pipe(command);
	else if (x == 6)
		exec_exit(command, env, tc);
	else if (x == 7)
		return_value = call_env(env);
	return (return_value);
}
