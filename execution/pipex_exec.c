/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 10:12:00 by acollon           #+#    #+#             */
/*   Updated: 2026/02/02 10:17:31 by radib            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_tc(t_f **tc)
{
	free_token_list(&(*tc)->tokens);
	free(*tc);
}

void	is_a_directory(char **args, t_env *env, t_f *tc, char **env_str)
{
	free_split(args);
	free_env(env);
	free_split(env_str);
	free(tc->cmds);
	free_token_list(&tc->tokens);
	free(tc);
	ft_putendl_fd("minishell: path: Is a directory", STDERR_FILENO);
	exit (126);
}

void	free_all(char **args, t_env *env, t_f *tc, char **env_str)
{
	free_split(args);
	free_env(env);
	free_split(env_str);
	free(tc->cmds);
	free_token_list(&tc->tokens);
	free(tc);
}

void	command_not_found(char **args, t_env *env, t_f *tc, char **env_str)
{
	ft_putstr_fd(args[0], STDERR_FILENO);
	ft_putendl_fd(": command not found", STDERR_FILENO);
	free_all(args, env, tc, env_str);
	exit (127);
}

int	px_exec(char **args, char **env_str, t_env *env, t_f *tc)
{
	char		*path;
	struct stat	st;

	if (!args || !args[0])
		return (0);
	path = px_find_path(args[0], env_str);
	if (!path)
		command_not_found(args, env, tc, env_str);
	if (stat(path, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
		{
			free(path);
			is_a_directory(args, env, tc, env_str);
		}
	}
	execve(path, args, env_str);
	perror("execve");
	free(path);
	free_all(args, env, tc, env_str);
	exit (126);
}
