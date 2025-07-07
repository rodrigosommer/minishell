/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:26:39 by hjuma             #+#    #+#             */
/*   Updated: 2024/10/31 12:46:38 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last;

	if (!new || !lst)
		return ;
	if (!*lst)
		*lst = new;
	else
	{
		last = *lst;
		while (last->next)
			last = last->next;
		last->next = new;
	}
}
/*
int main(void)
{
    t_list *head = NULL;
    t_list *new_node1;
    t_list *new_node2;

    new_node1 = ft_lstnew("first node");
    new_node2 = ft_lstnew("second node");

    ft_lstadd_back(&head, new_node1);
    ft_lstadd_back(&head, new_node2);

    // Now the list should contain new_node1 followed by new_node2
}
*/
