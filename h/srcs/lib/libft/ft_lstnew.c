/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjuma <hjuma@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 14:26:09 by hjuma             #+#    #+#             */
/*   Updated: 2024/10/21 14:37:18 by hjuma            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*node;

	node = malloc(sizeof(t_list));
	if (NULL == node)
		return (NULL);
	node->content = content;
	node->next = NULL;
	return (node);
}
/*
int main()
{
    // Create a new node with some content
    char *data = strdup("Hello, World!");
    t_list *node = ft_lstnew(data);
    
    // Check if the node was created successfully
    if (node)
    {
        printf("Node created with content: %s\n", (char *)node->content);
        // Remember to free the content and node later
        free(node->content);  // Free the duplicated string
        free(node);           // Free the node itself
    }
    else
    {
        printf("Failed to create node\n");
    }

    return 0;
}*/
