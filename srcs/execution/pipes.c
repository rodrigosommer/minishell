/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/05 16:23:12 by rsommer           #+#    #+#             */
/*   Updated: 2025/06/05 16:23:17 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void close_pipes(int **pipes, int num_pipes)
{
    int i;
    
    if (!pipes)
        return;
    
    i = 0;
    while (i < num_pipes)
    {
        if (pipes[i])
        {
            if (pipes[i][0] != -1)
                close(pipes[i][0]);
            if (pipes[i][1] != -1)
                close(pipes[i][1]);
            free(pipes[i]);
        }
        i++;
    }
    free(pipes);
}

static int **create_pipes(int num_pipes)
{
    int **pipes;
    int i;

    if (num_pipes <= 0)
        return NULL;

    pipes = malloc(sizeof(int *) * num_pipes);
    if (!pipes)
        return NULL;

    i = 0;
    while (i < num_pipes)
    {
        pipes[i] = malloc(sizeof(int) * 2);
        if (!pipes[i])
        {
            close_pipes(pipes, i);
            return NULL;
        }
        if (pipe(pipes[i]) == -1)
        {
            free(pipes[i]);
            close_pipes(pipes, i);
            print_error("pipe", NULL, strerror(errno));
            return NULL;
        }
        i++;
    }
    return pipes;
}

static int count_commands(t_cmd *cmd)
{
    int count = 0;
    while (cmd)
    {
        count++;
        cmd = cmd->next;
    }
    return count;
}

static void setup_child_pipes(int **pipes, int cmd_index, int num_commands)
{
    int i;

    // Set up input: read from previous pipe if not first command
    if (cmd_index > 0)
    {
        if (dup2(pipes[cmd_index - 1][0], STDIN_FILENO) == -1)
        {
            print_error("dup2", NULL, strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    // Set up output: write to next pipe if not last command  
    if (cmd_index < num_commands - 1)
    {
        if (dup2(pipes[cmd_index][1], STDOUT_FILENO) == -1)
        {
            print_error("dup2", NULL, strerror(errno));
            exit(EXIT_FAILURE);
        }
    }

    // Close all pipe file descriptors in child
    i = 0;
    while (i < num_commands - 1)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
        i++;
    }
}

int handle_pipes(t_cmd *cmd, t_env *env, t_data *data)
{
    int num_commands = count_commands(cmd);
    int **pipes;
    pid_t *pids;
    t_cmd *current = cmd;
    int i = 0;
    int final_status = 0;

    debug_log("handle_pipes", "Starting pipeline", ft_itoa(num_commands));

    if (num_commands < 2)
        return execute_command(cmd, env, data);

    // Create pipes
    pipes = create_pipes(num_commands - 1);
    if (!pipes)
    {
        data->exit_status = 1;
        return 1;
    }

    // Create array to store child PIDs
    pids = malloc(sizeof(pid_t) * num_commands);
    if (!pids)
    {
        close_pipes(pipes, num_commands - 1);
        data->exit_status = 1;
        return 1;
    }

    // Fork all processes
    while (current && i < num_commands)
    {
        pids[i] = fork();
        
        if (pids[i] == -1)
        {
            print_error("fork", NULL, strerror(errno));
            // Kill already forked children
            for (int j = 0; j < i; j++)
                kill(pids[j], SIGTERM);
            // Wait for them to die
            for (int j = 0; j < i; j++)
                waitpid(pids[j], NULL, 0);
            close_pipes(pipes, num_commands - 1);
            free(pids);
            data->exit_status = 1;
            return 1;
        }
        
        if (pids[i] == 0)
        {
            // Child process
            setup_child_signals();
            
            // Set up pipes first
            setup_child_pipes(pipes, i, num_commands);
            
            // Then handle explicit redirections (they override pipes)
            // Check for redirection failures - if any fail, exit with status 1
            // but don't prevent other commands in pipeline from running
            if (current->in_fd != STDIN_FILENO && current->in_fd != -1)
            {
                if (dup2(current->in_fd, STDIN_FILENO) == -1)
                {
                    print_error("dup2", NULL, strerror(errno));
                    exit(EXIT_FAILURE);
                }
                close(current->in_fd);
            }
            else if (current->in_fd == -1)
            {
                // Input redirection failed during parsing
                // Exit this command but don't stop the pipeline
                exit(1);
            }
            
            if (current->out_fd != STDOUT_FILENO && current->out_fd != -1)
            {
                if (dup2(current->out_fd, STDOUT_FILENO) == -1)
                {
                    print_error("dup2", NULL, strerror(errno));
                    exit(EXIT_FAILURE);
                }
                close(current->out_fd);
            }
            else if (current->out_fd == -1)
            {
                // Output redirection failed during parsing
                // Exit this command but don't stop the pipeline  
                exit(1);
            }

            // Close extra file descriptors
            for (int fd = 3; fd < 1024; fd++)
                close(fd);

            // Execute command
            execute_child(current, env, data);
            exit(127);
        }

        current = current->next;
        i++;
    }

    // Parent: close all pipe file descriptors
    close_pipes(pipes, num_commands - 1);

    // Wait for all children and collect exit statuses
    i = 0;
    while (i < num_commands)
    {
        int status;
        waitpid(pids[i], &status, 0);
        
        // The exit status of a pipeline is the exit status of the last command
        if (i == num_commands - 1)
        {
            if (WIFEXITED(status))
            {
                final_status = WEXITSTATUS(status);
            }
            else if (WIFSIGNALED(status))
            {
                int sig = WTERMSIG(status);
                if (sig == SIGINT)
                    write(STDOUT_FILENO, "\n", 1);
                else if (sig == SIGQUIT)
                    ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
                final_status = 128 + sig;
            }
        }
        else
        {
            // For non-last commands, just handle signal display
            if (WIFSIGNALED(status))
            {
                int sig = WTERMSIG(status);
                if (sig == SIGINT && i == 0) // Only first command shows signal
                    write(STDOUT_FILENO, "\n", 1);
                else if (sig == SIGQUIT && i == 0)
                    ft_putstr_fd("Quit: 3\n", STDERR_FILENO);
            }
        }
        i++;
    }

    free(pids);
    data->exit_status = final_status;
    debug_log("handle_pipes", "Pipeline completed", ft_itoa(final_status));
    return final_status;
}
