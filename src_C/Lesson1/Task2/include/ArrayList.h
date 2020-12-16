#ifndef _ARRAY_LIST_
#define _ARRAY_LIST_

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct arr_list
{
	void** data;
	size_t size;
}arrlist;

struct arr_list createStruct (void* item);
void add(struct arr_list* list, void* item);
void insert(struct arr_list* list, int index, void* item);
size_t sizeArr(struct arr_list* list);
void removeList(struct arr_list* list, int index);
void set(struct arr_list* list, int index, void* item);
void* get(struct arr_list* list, int index);
void destructor(struct arr_list* list);

#endif
