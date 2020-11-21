#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <math.h>

typedef struct{
	double x;
	double y;
}point;

double Random(double min, double max){
    return (double)(rand())/RAND_MAX*(max - min) + min;
}

void generatePoints(point** points, double radius, int number){
	
	struct timespec now;
	if(clock_gettime(CLOCK_REALTIME, &now) < 0){
		fprintf(stderr, "Error time\n");
		exit(EXIT_FAILURE);
	}
	srand(now.tv_nsec);	

	(*points)[0].x = Random(0.0-radius, 0.0+radius);
	(*points)[0].y = Random(0.0-radius, 0.0+radius);
	for(int i = 1; i < number; i++){
		do{
			(*points)[i].x = Random(0.0-radius, 0.0+radius);
			(*points)[i].y = Random(0.0-radius, 0.0+radius);
		}while((*points)[i].x == (*points)[i-1].x && (*points)[i].y == (*points)[i-1].y);
	}
}

double getDistance(point pt){
	return sqrt(pt.x*pt.x + pt.y*pt.y);
}

int main(int argc, char* argv[]){ 
	double radius = 1.0;
	int number = 10;
		
	int opt;
	while((opt = getopt(argc, argv, "r:n:")) != -1){
		switch(opt){
			case 'r':
				radius = atof(optarg);
				break;
			case 'n':
				number = atoi(optarg);
				break;
		}
	}

	point* points = (point*) malloc(number * sizeof(point)); 
	generatePoints(&points, radius, number);
	
	int rightsPointsCounter = 0;
	for(int i = 0; i < number; i++)
		if(getDistance(points[i]) <= radius)
			rightsPointsCounter++;
	free(points);
	exit(rightsPointsCounter);
}

