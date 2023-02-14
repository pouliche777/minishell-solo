/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bperron <bperron@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 16:39:06 by slord             #+#    #+#             */
/*   Updated: 2023/02/09 10:20:48 by bperron          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void children(t_shell *shell, int i)
{
	signals(2);
	change_in_and_out(shell, i);
	if (!check_output(shell, i))
		return ;
	if (!check_input(shell, i))
		return ;
	supress_operators(shell, i);
	if (check_built_in(shell, i) == 0)
	{
		if (modify_command(shell) == 0)
			printf("minishell: %s : command not found\n", shell->cmds[i][0]);
		execve(shell->cmds_exe[0], shell->cmds_exe, shell->env);
	}
}

void	execute(t_shell *shell)
{
	int	i;

	i = 0;
	while (i < shell->nb_cmds)
	{
		check_quotes(shell, i, 0);
		check_dollar_in_command(shell, i, shell->cmds[i]);
		check_built_in_parent(shell, i);
		check_heredoc_parent(shell, i);
		shell->id[i] = fork();
		if (shell->id[i] == 0)
		{
			signals(2);
			children(shell, i);
			return ;
		}
		signals(0);
		close(shell->fd[(i * 2) + 1]);
		if (i > 0)
			close(shell->fd[i * 2 - 2]);
		i++;
	}
	i = -1;
	while (shell->id[++i])
		waitpid(shell->id[i], &shell->status, 0);
//	launch_terminal(shell);
}

void	launch_terminal(t_shell *shell)
{
	while (1)
	{
		if (shell->buffer)
		{
			free(shell->buffer);
			shell->buffer = NULL;
		}
		shell->buffer = readline("~");
		if (shell->buffer == NULL)
			exit(0);
		add_history(shell->buffer);
		lexer(shell->buffer, shell);
		//set_pipes(shell);
		//execute(shell);
	}
}

void	init_env(t_shell *shell, char **env)
{
	int		j;

	j = 0;
	while (env[j])
		j++;
	j--;
	shell->env = ft_calloc((j + 2), sizeof(char *));
	while (j >= 0)
	{
		shell->env[j] = ft_strdup(env[j]);
		j--;
	}
}

int	main(int argc, char **argv, char **env)
{
	t_shell	*shell;

	if (argc == 1 && argv[0])
	{
		shell = get_struc();
		init_env(shell, env);
		get_path(shell);
		launch_terminal(shell);
	}
}

t_shell	*get_struc(void)
{
	static t_shell	*shell = NULL;

	if (shell == NULL)
		shell = malloc(sizeof(t_shell));
	return (shell);
}
