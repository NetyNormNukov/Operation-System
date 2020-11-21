#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "point.h"

int main(void)
{	
	int n;
	printf("Enter value points\n");
	scanf("%d", &n);
	struct point tmp;
	struct point* arr = (point*)malloc(n* sizeof(point));

	fill_arr(arr,n);

	while(n > 1)
	{

    printArr(arr, n);

    int m1 = find_min(arr, n);
    tmp = arr[m1];
    arr[m1] = arr[n-1];
    arr[n-1] = tmp;

    int m2 = find_min_neighbor(arr, n, n-1);
    arr[m2].mas+= arr[n-1].mas;
    
    n--;

   	arr = realloc(arr, n * sizeof(point));

   	// printf("Point: "); 
   	
   	printf("%lu\n", sizeof(arr)/sizeof(point));    
  	}
  	printf("%.2f %.2f %.2f %.2f\n",arr[0].x,arr[0].y,arr[0].z,arr[0].mas);
  	free(arr);
	
	return EXIT_SUCCESS;
}

