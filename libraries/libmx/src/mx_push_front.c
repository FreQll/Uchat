#include "../inc/libmx.h"

void mx_push_front(t_list ** list, void *data){
	if (!*list){
		*list = mx_create_node(data);
	}else{
		t_list * prev = mx_create_node(data);
		prev->next = *list;
		*list = prev;
	}
}

