#ifndef _POINT_H_
#define _POINT_H_

typedef struct point{
	float x,y,z,mas;
}point, *ppoint;

float Distance(struct point p, struct point q);
void fill_arr(point* str,int n);
void printArr(point* arr, int n);
int find_min(point* str,int n);
int find_min_neighbor(point* arr,int n,int j);

#endif
