#include "node.h"

NNODE *
node_add (NNODE *head, int type, void *data)
{
	NNODE *n_node = (NNODE *) malloc (sizeof (NNODE));

	n_node->next = head;
	n_node->data = data;
	n_node->type = type;

	return n_node;
}

int
node_get_type (NNODE *head)
{	
	return head->type;
}

void *
node_get_data (NNODE *n)
{
	return n->data;
}

int
node_delete (NNODE *head, int num)
{
	NNODE *cur = head;
	NNODE *prev = NULL;
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

NNODE *
node_get_next (NNODE *n)
{
	return n->next;
}

NNODE *
node_clear (NNODE *head)
{
	NNODE *tmp = NULL;
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
