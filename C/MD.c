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


//void evolve(int count, double dt, double timing[]) {
void evolve(int count, double dt, double timing[]) {
	int step;
	int i, j, k, l;
	double start, stop;

	/*
	 * Loop over timesteps.
	 */
	for (step = 1; step <= count; step++) {

		printf("timestep %d\n",step);
		printf("collisions %d\n",collisions);


		/* set the viscosity term in the force calculation */
		start = second();
		for (j = 0; j < Ndim; j++) {
			visc_force(Nbody, f[j], visc, vel[j]);
		}
		stop = second();

		timing[0] += stop - start;


		/* add the wind term in the force calculation */
		start = second();

		wind_force(Nbody, f, visc, pos, P);

		stop = second();

		timing[1] += stop - start;


		/* calculate distance from central mass */
		start = second();
		for (k = 0; k < Nbody; k++) {
			r[k] = 0.0;
		}

		for (i = 0; i < Ndim; i++) {
			add_norm(Nbody, r, pos[i]);
		}

		for (k = 0; k < Nbody; k++) {
			r[k] = sqrt(r[k]);
		}
		stop = second();

		timing[2] += stop - start;


		/* calculate central force */
		start = second();
		for (i = 0; i < Nbody; i++) {
			for (l = 0; l < Ndim; l++) {
				f[l][i] = f[l][i] - 
					force(G*mass[i]*M_central, pos[l][i], r[i]);
			}
		}
		stop = second();

		timing[3] += stop - start;


		/* calculate pairwise separation of particles */
		start = second();
		k = 0;
		for (i = 0; i < Nbody; i++) {
			for (j = i+1; j < Nbody; j++) {
				for (l = 0; l < Ndim; l++) {
					delta_x[l][k] = pos[l][i] - pos[l][j];
				}
				k = k + 1;
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
			add_norm(Npair, delta_r, delta_x[i]);
		}
		for (k = 0; k < Npair; k++) {
			delta_r[k] = sqrt(delta_r[k]);
		}
		stop = second();

		timing[5] += stop - start;


		/*
		 * add pairwise forces.
		 */
		start = second();
		k = 0;
		for (i = 0; i < Nbody; i++) {
			for (j = i+1; j<Nbody; j++) {
				for (l = 0; l < Ndim; l++) {
					/*  flip force if close in */
					if ( delta_r[k] >= Size ) {
						f[l][i] = f[l][i] - 
							force(G*mass[i]*mass[j], delta_x[l][k], delta_r[k]);
						f[l][j] = f[l][j] + 
							force(G*mass[i]*mass[j], delta_x[l][k], delta_r[k]);
					}
					else {
						f[l][i] = f[l][i] + 
							force(G*mass[i]*mass[j], delta_x[l][k], delta_r[k]);
						f[l][j] = f[l][j] - 
							force(G*mass[i]*mass[j], delta_x[l][k], delta_r[k]);
						collisions++;
					}
				}
				k = k + 1;
			}
		}
		stop = second();

		timing[6] += stop - start;


		/* update positions */
		start = second();
		for (i = 0; i < Nbody; i++) {
			for (j = 0; j < Ndim; j++) {
				pos[j][i] = pos[j][i] + dt * vel[j][i];
			}
		}
		stop = second();

		timing[7] += stop - start;


		/* update velocities */
		start = second();
		for (i = 0; i < Nbody; i++) {
			for (j = 0; j < Ndim; j++) {
				vel[j][i] = vel[j][i] + dt * (f[j][i]/mass[i]);
			}
		}
		stop = second();

		timing[8] += stop - start;

	}

}




