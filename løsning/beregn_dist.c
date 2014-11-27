#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define JORDENS_RADIUS 6373
#define PI 3.141592653589793
#define GRAD_TIL_RAD 0.017453292519943295769236907684886

double beregn_dist(double lat1, double lon1, double lat2, double lon2);

int main()
{
	double lat1 = (57.041256), lon1 = (9.875491000000011),  lat2 = (57.036041), lon2 = (9.95243800000003);
	double dist = beregn_dist(lat1, lon1, lat2, lon2);
	printf("%f\n", dist);
	return 0;
}

double beregn_dist(double lat1, double lon1, double lat2, double lon2)
{
	double forsteDel = pow(sin((lat2 - lat1)/2 * GRAD_TIL_RAD),2);
	double andenDel = pow(sin((lon2 - lon1)/2 * GRAD_TIL_RAD),2);
	double a = sqrt(forsteDel + cos(lat1 * GRAD_TIL_RAD) * cos(lat2 * GRAD_TIL_RAD) * andenDel);
	return 2 * JORDENS_RADIUS * asin(a);
}