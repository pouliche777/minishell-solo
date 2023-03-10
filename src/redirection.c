/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slord <slord@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 19:11:43 by slord             #+#    #+#             */
/*   Updated: 2023/03/01 14:15:42 by slord            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/shell.h"

void	redirect_output(char *cmd)
{
	int	file[2];
	int	i;

	if (cmd[0] == '\'' || cmd[0] == '\"')
		cmd++;
	i = 0;
	while (cmd[i])
	{
		if (cmd[i + 1] == '\0')
		{
			if (cmd[i] == '\'' || cmd[i] == '\"')
				cmd[i] = '\0';
		}
		i++;
	}
	if (cmd)
	{
		file[0] = open(cmd, O_WRONLY | O_CREAT | O_TRUNC, 0777);
		dup2(file[0], STDOUT_FILENO);
		close (file[0]);
	}
}

void	redirect_output_1(char *cmd)
{
	int	file[2];
	int	i;

	if (cmd[0] == '\'' || cmd[0] == '\"')
		cmd++;
	i = 0;
	while (cmd[i])
	{
		if (cmd[i + 1] == '\0')
		{
			if (cmd[i] == '\'' || cmd[i] == '\"')
				cmd[i] = '\0';
		}
		i++;
	}
	if (cmd)
	{
		file[0] = open (cmd, O_CREAT | O_APPEND | O_WRONLY, 0777);
		dup2(file[0], STDOUT_FILENO);
		close (file[0]);
	}
}

void	redirect_input(char *cmd)
{
	int	file[2];
	int	i;

	if (cmd[0] == '\'' || cmd[0] == '\"')
		cmd++;
	i = 0;
	while (cmd[i])
	{
		if (cmd[i + 1] == '\0')
		{
			if (cmd[i] == '\'' || cmd[i] == '\"')
				cmd[i] = '\0';
		}
		i++;
	}
	if (cmd)
	{
		file[0] = open (cmd, O_RDONLY, 0777);
		dup2(file[0], STDIN_FILENO);
		close (file[0]);
	}
}

int	check_output(t_shell *shell, int i)
{
	int		j;
	char	**cmd;

	cmd = shell->cmds[i];
	j = 0;
	while (cmd[j])
	{
		if (cmd[j][0] == '>' && cmd[j][1] == '>' && cmd[j + 1] != NULL)
		{
			redirect_output_1(cmd[j + 1]);
			if (cmd[j + 1] == NULL)
				return (0);
		}
		else if (cmd[j][0] == '>' && cmd[j][1] == '\0' && cmd[j + 1] != NULL)
		{
			redirect_output(cmd[j + 1]);
			if (cmd[j + 1] == NULL)
				return (0);
		}
		j++;
	}
	return (1);
}

int	check_input(t_shell *shell, int i)
{
	int		j;
	char	**cmd;

	cmd = shell->cmds[i];
	j = 0;
	while (cmd[j])
	{
		if (cmd[j][0] == '<' && cmd[j][1] == '\0')
			redirect_input(cmd[j + 1]);
		else if (cmd[j][0] == '<' && cmd[j][1] == '<' && cmd[j][2] == '\0')
		{
			dup2(shell->heredoc_fd[0], STDIN_FILENO);
			close(shell->heredoc_fd[0]);
			close(shell->heredoc_fd[1]);
		}	
		j++;
	}
	return (1);
}
