#include "./include/ArrayList.h"

int main(int argc,char** argv){

	int a = 10;
	double b = 15.1;
	char ch[1] = "j";
	char* arrchar = "fghjk";
	int inter = 1000;
	struct arr_list list = createStruct(&a);	//create
	printf("%i\n", *((int*)get(&list,0)));
	set(&list,0,&b);							//set
	printf("%3f\n", *((double*)get(&list,0)));

	int size = sizeArr(&list);					//size
	printf("%i\n",size);

	add(&list, &ch);							//add
	add(&list, &arrchar);
	printf("%c\n", *((char*)get(&list,1)));
	printf("%s\n", *((char**)get(&list,2)));

	size = sizeArr(&list);	
	printf("%i\n",size);

	insert(&list,1,&inter);						//insert
	printf("%i\n",*((int*)get(&list,1)));

	size = sizeArr(&list);	
	printf("%i\n",size);

	removeList(&list,4);						//remove
	// printf("%i\n",*((int*)get(&list,4))); не сработает)

	size = sizeArr(&list);	
	printf("size = %i\n",size);
	// printf("Value in [1]%3f\n", *((double*)get(&list,1)));
	// printf("%i\n", *((int*)get(&list,1)));
	printf("bef%s\n", *((char**)get(&list,2)));

	// for (int i = 0; i < list.size-1; ++i)
	// {	
	// 	printf("size loop = %d\n", i);
	// 	free(list.data[i+1]);
	// }
	destructor(&list);
	// free(list.data);

	return EXIT_SUCCESS;

}