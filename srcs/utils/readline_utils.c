/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rsommer <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/16 00:39:27 by rsommer           #+#    #+#             */
/*   Updated: 2025/06/16 00:39:29 by rsommer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include <readline/readline.h>
#include <readline/history.h>

void clean_readline(void)
{
    // Clear any pending input
    rl_free_line_state();
    rl_clear_signals();
    rl_cleanup_after_signal();
    
    // Free history entries
    clear_history();
    
    // Free readline internal state
    rl_clear_history();
}
