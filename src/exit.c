/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperron <bperron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 13:12:46 by bperron           #+#    #+#             */
/*   Updated: 2023/03/01 13:13:18 by bperron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/shell.h"

int	check_arg(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (ft_isdigit(arg[i]) == 0)
			return (255);
		i++;
	}
	if (i > 19)
		return (20);
	else
		return (i);
}

void	set_status(char *arg, unsigned char *status)
{
	if (check_arg(arg) > 19)
	{
		errno = 22;
		dprintf(2, "Minishell: exit: %s: ", arg);
		perror("");
		*status = 255;
	}
	else
		*status = atoll(arg);
}

void	exit_built_in(t_shell *shell, char **cmd)
{
	unsigned char	status;
	int				args;

	status = 0;
	printf("exit\n");
	args = arr_size(cmd);
	if (args <= 2)
	{
		if (args == 2)
			set_status(cmd[1], &status);
		else if (args == 1)
			status = shell->status;
		free_garbage(shell, status);
	}
	else
	{
		errno = 7;
		shell->status = 256;
		perror("Minishell: exit");
	}
}
