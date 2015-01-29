#include <stdio.h>
#include <stdlib.h>

#include "list.h"

struct cont {
	int	data;
	struct	list_head list;
}; 

int main (int arg, char **argv)
{
	int	i;
	struct	cont *node;
	struct	cont *n;

	LIST_HEAD (list_h);

	for (i = 0; i < 10; i++) {
		node = (struct cont *) malloc (sizeof (*node));
		if (!node) {
			printf ("node malloc error\n");
			return 0;
		}

		node->data = i;
		INIT_LIST_HEAD (&node->list);

		list_add (&node->list, &list_h);
	}

	node = NULL;
	list_for_each_entry_safe (node, n, &list_h, list) {
		printf ("%d\n", node->data);

		if (node->data % 2 == 0) {
			list_del (&node->list);
			list_add (&node->list, &list_h);
		}
	}

	printf ("odd\n");
	node = NULL;
	list_for_each_entry (node, &list_h, list)
		printf ("%d\n", node->data);

	printf ("reverse\n");
	node = NULL;
	list_for_each_entry_reverse (node, &list_h, list)
		printf ("%d\n", node->data);

	return 0;
}
