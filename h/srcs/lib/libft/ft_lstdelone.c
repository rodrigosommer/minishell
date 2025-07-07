/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:25:26 by hjuma             #+#    #+#             */
/*   Updated: 2024/10/21 14:37:28 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstdelone(t_list *lst, void (*del)(void*))
{
	if (!lst || !del)
		return ;
	del(lst->content);
	free(lst);
}
/*

int	main(void)
{
    t_list	*node;

    node = ft_lstnew(strdup("Hello, world!")); 
     // Create a new node with dynamically allocated content

    // Delete the node and free its content
    ft_lstdelone(node, del);

    // At this point, 'node' has been freed and should not be used
}
*/
