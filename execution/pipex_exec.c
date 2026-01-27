/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 10:12:00 by acollon           #+#    #+#             */
/*   Updated: 2026/01/27 15:01:24 by radib            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_tc(t_f **tc)
{
	free_command_list(&(*tc)->cmds);
	free_token_list(&(*tc)->tokens);
	free(*tc);
}

int	px_exec(char **args, char **env_str, t_env *env, t_f *tc)
{
	char	*path;

	if (!args || !args[0])
		return (0);
	path = px_find_path(args[0], env_str);
	if (!path)
	{
		ft_putstr_fd("Command not found: ", STDERR_FILENO);
		ft_putendl_fd(args[0], STDERR_FILENO);
		free_split(args);
		free_env(env);
		free_split(env_str);
		free(tc->cmds);
		free_token_list(&tc->tokens);
		free(tc);
		exit (127);
	}
	execve(path, args, env_str);
	perror("execve");
	free_split(args);
	exit (126);
}
