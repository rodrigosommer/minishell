/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:26:23 by hjuma             #+#    #+#             */
/*   Updated: 2024/10/21 14:37:16 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int	counter;

	counter = 0;
	while (lst)
	{
		++counter;
		lst = lst->next;
	}
	return (counter);
}
/*
int main(void)
{
	// Create a simple linked list with three nodes
	t_list *node1 = ft_lstnew("Node 1");
	t_list *node2 = ft_lstnew("Node 2");
	
	node1->next = node2;
	
	// Get the size of the list
	int size = ft_lstsize(node1);
	printf("List size: %d\n", size);

	// Free the allocated memory
	free(node1);
	free(node2);
	
	return 0;
}
*/
