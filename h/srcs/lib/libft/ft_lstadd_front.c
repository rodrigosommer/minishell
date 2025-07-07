/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:26:47 by hjuma             #+#    #+#             */
/*   Updated: 2024/10/21 14:36:46 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (lst && new)
	{
		new->next = *lst;
		*lst = new;
	}
}
/*
int main(void)
{
    t_list *head = NULL;
    t_list *new_node1;
    t_list *new_node2;

    new_node1 = ft_lstnew("first node");
    new_node2 = ft_lstnew("new front node");

    ft_lstadd_front(&head, new_node1);
    ft_lstadd_front(&head, new_node2);

    // Now the list should have new_node2 at the front, followed by new_node1
}
*/
