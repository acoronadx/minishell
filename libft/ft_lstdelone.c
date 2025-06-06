/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarodri2 <rarodri2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 18:11:03 by rarodri2          #+#    #+#             */
/*   Updated: 2025/01/21 13:11:43 by rarodri2         ###   ########.fr       */
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
t_list *ft_lstnew(void *content)
{
    t_list *new_node;

    new_node = (t_list *)malloc(sizeof(t_list));
    if (!new_node)
        return (NULL);
    new_node->content = content;
    new_node->next = NULL;
    return (new_node);
}
void del(void *content)
{
    free(content);
}
void	ft_lstprint(t_list *lst)
{
	while (lst)
	{
		printf("%s -> ", (char *)lst->content);
		lst = lst->next;
	}
	printf("NULL\n");
}

int	main(void)
{
	t_list *head = ft_lstnew(strdup("First"));
	head->next = ft_lstnew(strdup("Second"));
	head->next->next = ft_lstnew(strdup("Third"));

	printf("The linked list before deletion: ");
	ft_lstprint(head);

	t_list *to_delete = head->next;
    head->next = to_delete->next; 

	ft_lstdelone(to_delete, del);

	printf("The linked list after deletion: ");
	ft_lstprint(head);

	free(head->content);
    free(head);

	return (0);
}
*/