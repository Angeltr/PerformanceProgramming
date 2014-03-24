/*
 *  Simple molecular dynamics code.
 *  $Id: MD-c.c,v 1.2 2002/01/31 16:43:14 spb Exp spb $
 *
 * This program implements:
 *     long range inverse square forces between particles. F = G * m1*m2 / r**2
 *     viscosity term     F = -u V
 * If 2 particles approach closer than Size we flip the direction of the
 * interaction force to approximate a collision.
 *
 * Coordinates are relative to a large central mass and the entire system is moving relative to the
 * viscous media.
 * If 2 particles approach closer than Size we flip the direction of the
 * interaction force to approximate a collision.
 *
 * This program was developed as part of a code optimisation course
 * and is therefore deliberately inefficient.
 *
 */
#include <math.h>
#include "coord.h"
#include "util.h"


void evolve(int count, double dt, double timing[]) {
	int step;
	int i, j, k, l;
	double start, stop;
	double rr, W;

	/*
	 * Loop over timesteps.
	 */
	for (step = 1; step <= count; step++) {

		printf("timestep %d\n",step);
		printf("collisions %d\n",collisions);


		/* set the viscosity term in the force calculation */
		start = second();
		for (j = 0; j < Ndim; j++) {
			for (i = 0; i < Nbody; i++) {
				f[j][i] = -visc[i] * vel[j][i];
			}
		}
		stop = second();

		timing[0] += stop - start;


		/* add the wind term in the force calculation */
		start = second();

		for (i = 0; i < Nbody; i++) {
			rr = 0.0;
			for (j = 0; j < 3; j++) {
				rr += pos[j][i] * pos[j][i];
			}

			f[0][i] = f[0][i] - ( visc[i] * pos[1][i] * P[2] )/rr;
			f[1][i] = f[1][i] + ( visc[i] * pos[2][i] * P[0] )/rr;
			f[2][i] = f[2][i] - ( visc[i] * pos[0][i] * P[1] )/rr;
		}


		stop = second();

		timing[1] += stop - start;


		/* calculate distance from central mass */
		start = second();
		for (k = 0; k < Nbody; k++) {
			r[k] = 0.0;
		}

		for (i = 0; i < Ndim; i++) {
			for (k = 0; k < Nbody; k++) {
				r[k] += (pos[i][k] * pos[i][k]);
			}
		}

		for (k = 0; k < Nbody; k++) {
			r[k] = sqrt(r[k]);
		}
		stop = second();

		timing[2] += stop - start;


		/* calculate central force */
		start = second();
		for (l = 0; l < Ndim; l++) {
			for (i = 0; i < Nbody; i++) {
				W = G*mass[i]*M_central;
				f[l][i] = f[l][i] - W*pos[l][i]/(pow(r[i],3.0));
			}
		}
		stop = second();

		timing[3] += stop - start;


		/* calculate pairwise separation of particles */
		start = second();
		k = 0;
		for (l = 0; l < Ndim; l++) {
			k = 0;
			for (i = 0; i < Nbody; i++) {
				for (j = i+1; j < Nbody; j++,k++) {
					delta_x[l][k] = pos[l][i] - pos[l][j];
				}
			}
		}
		stop = second();

		timing[4] += stop - start;


		/* calculate norm of seperation vector */
		start = second();
		for (k = 0; k < Npair; k++) {
			delta_r[k] = 0.0;
		}
		for (i = 0; i < Ndim; i++) {
			for (k = 0; k < Npair; k++) {
				delta_r[k] += (delta_x[i][k] * delta_x[i][k]);
			}

		}
		for (k = 0; k < Npair; k++) {
			delta_r[k] = sqrt(delta_r[k]);
		}
		stop = second();

		timing[5] += stop - start;



		int debug=0;
		/*
		 * add pairwise forces.
		 */

		start = second();
		k = 0;
		for (l = 0; l < Ndim; l++) {
			k = 0;	
			for (i = 0; i < Nbody; i++) {
				for (j = i+1; j < Nbody; j++, k++) {
					W = G*mass[i]*mass[j];
					if ( delta_r[k] >= Size ) {
						f[l][i] = f[l][i] - W*delta_x[l][k]/(pow(delta_r[k],3.0));
						f[l][j] = f[l][j] + W*delta_x[l][k]/(pow(delta_r[k],3.0));
					}
					else {
						f[l][i] = f[l][i] + W*delta_x[l][k]/(pow(delta_r[k],3.0));
						f[l][j] = f[l][j] - W*delta_x[l][k]/(pow(delta_r[k],3.0));
						collisions++;
					}
				}
				if(debug) printf("\n");
			}
			if(debug)	printf("\n");
		}
		stop = second();

		timing[6] += stop - start;



		/* update positions and velocities */
		start = second();
		for (j = 0; j < Ndim; j++) {
			for (i = 0; i < Nbody; i++) {
				pos[j][i] = pos[j][i] + dt * vel[j][i];
				vel[j][i] = vel[j][i] + dt * (f[j][i]/mass[i]);
			}
		}
		stop = second();

		timing[7] += stop - start;


	}

}




