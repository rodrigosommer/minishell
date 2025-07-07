/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:25:51 by hjuma             #+#    #+#             */
/*   Updated: 2024/10/21 14:37:23 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	if (NULL == lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}
/*

int	main(void)
{
    t_list	*node1;
    t_list	*node2;
    t_list	*node3;
    t_list	*last;

    node1 = ft_lstnew(strdup("Node 1"));
    node2 = ft_lstnew(strdup("Node 2"));
    node3 = ft_lstnew(strdup("Node 3"));

    node1->next = node2;
    node2->next = node3;

    last = ft_lstlast(node1);  // Get the last node
    printf("Last node content: %s\n", (char *)last->content);

    return (0);
}*/
