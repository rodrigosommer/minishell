/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:25:39 by hjuma             #+#    #+#             */
/*   Updated: 2024/10/21 14:37:26 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	if (!lst || !f)
		return ;
	while (lst)
	{
		f(lst->content);
		lst = lst->next;
	}
}
/*

int	main(void)
{
    t_list	*node1;
    t_list	*node2;
    t_list	*node3;

    node1 = ft_lstnew(strdup("Node 1"));
    node2 = ft_lstnew(strdup("Node 2"));
    node3 = ft_lstnew(strdup("Node 3"));

    node1->next = node2;
    node2->next = node3;

    ft_lstiter(node1, print_content);

    return (0);
*/
