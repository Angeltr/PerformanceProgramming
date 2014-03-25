#include <math.h>
#include "coord.h"
#include <sys/time.h>


void visc_force(int N, double *f, double *visc, double *vel) {
	int i;

	for (i = 0; i < N; i++) {
		f[i] = -visc[i] * vel[i];
	}
}


void wind_force(int N,double f[][Nbody], double visc[], double pos[][Nbody], double P[]) {
	int i, j;
	double r;

	for (i = 0; i < N; i++) {
		r = 0.0;
		for (j = 0; j < 3; j++) {
			r += pos[j][i] * pos[j][i];
		}

		f[0][i] = f[0][i] - ( visc[i] * pos[1][i] * P[2] )/r;
		f[1][i] = f[1][i] + ( visc[i] * pos[2][i] * P[0] )/r;
		f[2][i] = f[2][i] - ( visc[i] * pos[0][i] * P[1] )/r;
	}
}


void add_norm(int N,double *r, double *delta) {
	int k;

	for (k = 0; k < N; k++) {
		r[k] += (delta[k] * delta[k]);
	}
}


double force(double W, double delta, double r) {

	return W*delta/(pow(r,3.0));
}


double second()
{
        /* struct timeval { long        tv_sec; 
 *            long        tv_usec;        };
 *
 *                       struct timezone { int   tz_minuteswest;
 *                                  int        tz_dsttime;      };     */

        struct timeval tp;
        struct timezone tzp;
        int i;

        i = gettimeofday(&tp,&tzp);
        return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
}

