#include "node.h"

struct NODE {
	int type;
	void *data;
	NODE *next;
};

NODE *
node_add (NODE *head, int type, void *data)
{
	NODE *n_node = (NODE *) malloc (sizeof (NODE));

	n_node->next = head;
	n_node->data = data;
	n_node->type = type;

	return n_node;
}

int
node_get_type (NODE *head)
{	
	return head->type;
}

void *
node_get_data (NODE *n)
{
	return n->data;
}

int
node_delete (NODE *head, int num)
{
	NODE *cur = head;
	NODE *prev = NULL;
	int i = 0;

	if (!head)
		return 0;
	
	while (cur){
		
		if (i == num){
			
			if (cur->data)
				free (cur->data);
			
			prev = cur->next;
			free (cur);
		}

		prev = cur;
		cur = cur->next;
		i++;
	}
	
	return 1;
}

NODE *
node_get_next (NODE *n)
{
	return n->next;
}

NODE *
node_clear (NODE *head)
{
	NODE *tmp = NULL;
	int n_del = 0;

	while (head){
		tmp = head;
		free (head->data);
		head = head->next;
		free (tmp);
		n_del++;
	}
	
	return head;
}
