#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <getopt.h>
#include <math.h>

long glob = 0;



void help()
{
	printf("-n or --number  Set number of incr/decr\n");
	printf("-p or --pairs 	Set number of threads\n");
}

void* threadIncr(void* arg)
{
	long num = *(long*)arg;
	for (int i = 0; i < num; ++i)
	{
		glob++;
	}
}

void* threadDecr(void* arg)
{
	long num = *(long*)arg;
	for (int i = 0; i < num; ++i)
	{
		glob--;
	}
}

static double func(double x)
{
	return (4-x*x);
}
int main(int argc, char const *argv[])
{	

	double I = 0;
	double a = 0;
	double b = 2;
	double e = 1e-3;

	int numSplit = 10000;
	if (argc == 2)
	{
		numSplit = atoi(argv[1]);
	}

	double In = 0.0;
	double I2n = In*2;
	double h = (b-a)/numSplit;
	while(1)
	{
		double h = (b-a)/numSplit;
		for (double x = a; x <= b; x+=h)
		{
			I2n += func(x+h/2);
		}
		I2n*=h;

		if (fabs(In - I2n)<e)
		{	

			break;
		}
		else
		{
			
			In = I2n;
			I2n = 0.0;
			numSplit += 100;
		}
	}
	printf("%f\n", I2n);
	


	return EXIT_SUCCESS;
}