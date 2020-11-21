#include <stdio.h>
#include <stdlib.h>
#include "./include/ArrayList.h"


struct arr_list createStruct (void* item){

	arrlist list;
	list.data = (void*)malloc(sizeof(void*));
	list.size = 1;
	list.data[0] = item;

	return list ;

}

void add(struct arr_list* list, void* item){

	(*list).size++;
	(*list).data = realloc((*list).data,((*list).size)*sizeof(void*));
	(*list).data[((*list).size-1)] = item;
	
}

void insert(struct arr_list* list, int index, void* item){

	if(index > (*list).size ){
		return;
	}
	(*list).size++;
	(*list).data = realloc((*list).data,((*list).size)*sizeof(void*));
	for(int i = (*list).size-2; i >= index; i--){
    	(*list).data[i+1] = (*list).data[i];
	}
  	(*list).data[index]=item;

}

size_t sizeArr(struct arr_list* list){return (*list).size;}

void removeList(struct arr_list* list, int index){


	for (int i = (*list).size-2; i >= index; i--){
		(*list).data[i+1] = (*list).data[i];
	}
	(*list).data = realloc((*list).data,((*list).size - 1)*sizeof(void*));
	(*list).size--;
}

void set(struct arr_list* list, int index, void* item){

	(*list).data[index] = item;

}

void* get(struct arr_list* list, int index){

	return (*list).data[index];
	
}