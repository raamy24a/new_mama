/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interactive_shell.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 15:31:36 by acollon           #+#    #+#             */
/*   Updated: 2026/02/02 08:52:15 by radib            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*prompt_listener(void)
{
	char	*user_input;

	user_input = readline("minishell$ ");
	if (user_input && *user_input)
		add_history(user_input);
	return (user_input);
}

void	handler(int sig)
{
	if (sig == SIGINT)
	{
		g_last_status = 130;
		(void)sig;
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (sig == SIGQUIT)
	{
		printf("\033[2K");
		rl_on_new_line();
		rl_redisplay();
	}
}

void	handler_heredoc(int sig)
{
	(void)sig;
	g_last_status = 130;
	write(1, "\n", 1);
	close(STDIN_FILENO);
}

int	interactive_shell(t_env *env)
{
	char				*user_input;
	struct sigaction	sa;

	g_last_status = 0;
	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	while (1)
	{
		sigaction(SIGINT, &sa, NULL);
		signal(SIGQUIT, SIG_IGN);
		user_input = prompt_listener();
		if (!user_input)
			break ;
		if (*user_input)
			g_last_status = prompt_execution(user_input, env, g_last_status);
		free(user_input);
	}
	ft_putendl_fd("exit", STDOUT_FILENO);
	return (g_last_status);
}

int	apply_redirection_only_command(t_f **tc, int pipefd[2],
		int prev_fd, t_env *env)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (perror("fork"), -1);
	if (pid == 0)
	{
		g_last_status = 0;
		apply_redirection_helper(tc, pipefd, prev_fd, env);
		if (g_last_status == 130)
			exit_call_silent (130, env, *tc);
		exit_call_silent (0, env, *tc);
	}
	return (pid);
}
