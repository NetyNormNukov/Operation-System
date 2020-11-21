#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "point.h"

void printArr(struct point* arr, int n){
	printf("________________________________________\n");
	for (int i = 0; i < n; ++i)
	{
		printf("Point%d(x=%f, y=%f, z=%f, weight=%f)\n", i, arr[i].x, arr[i].y, arr[i].z, arr[i].mas);
	}
	printf("________________________________________\n");
}

void fill_arr(point* str, int n){
	srand(time(NULL));
	for (int i = 0; i < n; ++i)
	{
		str[i].x = rand()%100;
		str[i].y = rand()%100;
		str[i].z = rand()%100;
		str[i].mas = rand()%100;
	}
}

int find_min(point* str, int n){
	float min_m = str[0].mas;
  	int ind = 0;
  	for(int i = 0; i < n; i++){
    	if(str[ind].mas < min_m){
      	min_m = str[ind].mas;
      	ind = i;
    	}
	}
  	return ind;
}

float Distance(struct point p, struct point q){
	return sqrt((p.x-q.x)*(p.x-q.x)+(p.y-q.y)*(p.y-q.y)+(p.z-q.z));
}

int find_min_neighbor(point* arr, int n, int j){

	float min_r = 0;
  	int ind = 0;
  	if(ind == j)ind++;
  		min_r = Distance(arr[j],arr[ind]);
  	for(int i = 0; i<n; i++){
    	if(i!=j && min_r<Distance(arr[j],arr[i])){
      	min_r = Distance(arr[j],arr[i]);
      	ind=i;
    	}
  	}
  return ind;
}
