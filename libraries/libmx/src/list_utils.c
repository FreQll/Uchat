#include "../inc/libmx.h"

void mx_clear_list(t_list **list, void(*free_content)(void * ptr)){
	if(!list) return;
	t_list * temp = *list;
	t_list * copy = temp;
	while (temp){
		copy = temp->next;
		if (free_content){
			free_content(temp->data);
		}
		free(temp);
		temp = copy;
	}
	*list = temp;
}

int max_len_name(t_list * list){
	if (!list) return 0;
	int max = mx_strlen(list->data);
	//char * t;
	for (t_list * temp = list; temp; temp = temp->next)
	{
		//printf("Entities: %s <-> %d\n", temp->data, mx_strlen(temp->data));
		if (max < mx_strlen(temp->data))
		{
			max = mx_strlen(temp->data);
		}
	}
	return max;
}

double aver_len_name(t_list * list){
	if (!list) return 0;
	double sum = 0;
	for (t_list * temp = list; temp; temp = temp->next)
	{
		sum += mx_strlen(temp->data);
	}
	return sum / mx_list_size(list);
}

void * find_by_i(t_list * list, int i){
	int j = 0;
	for (t_list*temp = list; temp; temp = temp->next){
		if (j++ == i) return temp->data;
	}
	return NULL;
}


