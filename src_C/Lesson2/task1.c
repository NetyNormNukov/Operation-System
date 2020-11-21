#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

double solveExp(double a, double b, double c);

int main(int argc, char** argv){
	if(argc != 4)
		printf("%s\n", "Enter 3 arguments.");
	double D = solveExp(atof(argv[1]), atof(argv[2]), atof(argv[3]));
	double X;
	printf("D: %2.2f\n",D);
	if(D == 0){
		X = (-1*atof(argv[2]))/(2*atof(argv[1]));
		
		printf("%2.2f\n", X);
	}
	else if(D < 0)
		printf("%s\n", "x not exist");
	else{
			X = ((-1*atof(argv[2])) + sqrt(D))/(2*atof(argv[1]));
			double X2 = ((-1*atof(argv[2])) - sqrt(D))/(2*atof(argv[1]));
			printf("x1 = %2.2f, x2 = %2.2f\n", X, X2);
	}

	printf("entered in ax^2 + bx + c:\n");
	printf("a: %2.2f, b: %2.2f, c: %2.2f\n",atof(argv[1]), atof(argv[2]), atof(argv[3]) );
	return 0;
}

double solveExp(double a, double b, double c){
	double D = (b*b) - (4*a*c);
	return D;
}