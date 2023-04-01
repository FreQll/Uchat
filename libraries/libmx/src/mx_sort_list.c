#include "../inc/libmx.h"

void swap_nums(t_list *a, t_list *b){
	void * temp = a->data;
	a->data = b->data;
	b->data = temp;
}

t_list *mx_sort_list(t_list *list, bool(*cmp)(void*a, void*b), bool reversed)
{
	t_list * init = list;  
	for (t_list * temp_i = list; temp_i; temp_i = temp_i->next) {
		for (t_list * temp_j = init; temp_j->next; temp_j = temp_j->next)
		{
			if (!reversed)
			{
				if (cmp(temp_j->data, temp_j->next->data))
				{	
					swap_nums(temp_j, temp_j->next);
				}		
			}
			else
			{
				if (!cmp(temp_j->data, temp_j->next->data))
				{	
					swap_nums(temp_j, temp_j->next);
				}	
			}	
		}
	}
	return list;
}


