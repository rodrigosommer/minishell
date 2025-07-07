/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 10:19:09 by hjuma             #+#    #+#             */
/*   Updated: 2024/10/30 10:22:05 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*next;
	t_list	*current;

	if (lst && del)
	{
		current = *lst;
		while (current)
		{
			next = current->next;
			del(current->content);
			free(current);
			current = next;
		}
		*lst = NULL;
	}
}
/*
int	main(void)
{
    t_list	*node1;
    t_list	*node2;
    t_list	*node3;

    node1 = ft_lstnew(strdup("First node"));
    node2 = ft_lstnew(strdup("Second node"));
    node3 = ft_lstnew(strdup("Third node"));

    node1->next = node2;
    node2->next = node3;

    // Clear and free the entire list
    ft_lstclear(&node1, del);

    // After this, node1 (and the entire list) 
    is set to NULL and memory is freed.
}
*/