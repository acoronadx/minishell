/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarodri2 <rarodri2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 11:13:47 by rarodri2          #+#    #+#             */
/*   Updated: 2025/01/21 14:51:56 by rarodri2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list;
	t_list	*new_node;

	new_list = NULL;
	while (lst)
	{
		new_node = ft_lstnew(f(lst->content));
		if (!new_node)
		{
			ft_lstclear(&new_list, del);
			return (NULL);
		}
		ft_lstadd_back(&new_list, new_node);
		lst = lst->next;
	}
	return (new_list);
}
/*
void	del(void *content)
{
	free(content);
}

void	*increment(void *content)
{
	int	*new_content;

	new_content = (int *)malloc(sizeof(int));
	if (new_content == NULL)
		return (NULL);
	*(new_content) = *((int *)content) + 1;
	return (new_content);
}
void	ft_lstprint(t_list *lst)
{
	while (lst)
	{
		printf("%d -> ", *((int *)lst->content));
		lst = lst->next;
	}
	printf("NULL\n");
}
int	main(void)
{
	t_list *list = NULL;

	int *num1 = (int *)malloc(sizeof(int));
	*num1 = 42;
	int *num2 = (int *)malloc(sizeof(int));
	*num2 = 36;
	int *num3 = (int *)malloc(sizeof(int));
	*num3 = 58;

	ft_lstadd_front(&list, ft_lstnew(num1));
	ft_lstadd_front(&list, ft_lstnew(num2));
	ft_lstadd_front(&list, ft_lstnew(num3));

	printf("Original list:\n");
    ft_lstprint(list);

    t_list *new_list = ft_lstmap(list, increment, del);

    printf("\nNew list after incrementing:\n");
    ft_lstprint(new_list);

    while (list)
    {
        t_list *tmp = list->next;
        del(list->content);
        free(list);
        list = tmp;
    }

    while (new_list)
    {
        t_list *tmp = new_list->next;
        del(new_list->content);
        free(new_list);
        new_list = tmp;
    }
	return (0);
}
*/