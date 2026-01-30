/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radib <radib@student.s19.be>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 17:48:51 by radib             #+#    #+#             */
/*   Updated: 2026/01/30 12:02:25 by radib            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../libft/libft.h"

t_long_verif	*ft_verif_atoll(const char *nptr
	, int sign, long long total, int i)
{
	t_long_verif	*nbr;

	nbr = malloc(sizeof (t_long_verif));
	nbr->status = 1;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == ' ')
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign *= -1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		total *= 10;
		total += nptr[i] - '0';
		if ((total * sign > 0) && sign == -1)
			nbr->status = 0;
		if ((total < 0) && sign == 1)
			nbr->status = 0;
		i++;
	}
	nbr->nbr = total * sign;
	return (nbr);
}

int	exit_call_silent(int x, t_env *env, t_f *tc)
{
	if (tc)
		free_command_list(tc->cmd_adress);
	free_env(env);
	if (tc)
		free_tc(&tc);
	rl_clear_history();
	x = x % 256;
	exit (x);
}

int	exit_call(long long x, t_env *env, t_f *tc)
{
	if (tc)
		free_command_list(tc->cmd_adress);
	free_env(env);
	if (tc)
		free_tc(&tc);
	rl_clear_history();
	x = x % 256;
	printf("exit\n");
	exit (x);
}
