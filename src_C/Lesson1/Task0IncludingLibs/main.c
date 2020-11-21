#include <stdio.h>
#include <stdlib.h>
#include "functs.h"

double getX(void);

int main(void) {
	double x;
	x = getX();
	printf("f(%5.2f) = %g\t\tg(%5.2f) = %g\n", x, f(x), x, g(x));

	return EXIT_SUCCESS;
}
	double getX(void) {
	double x;
	printf("x: ");
	scanf("%lf", &x);
	return x;
}