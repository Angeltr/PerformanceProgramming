/*
 * $Id: control-c.c,v 1.2 2002/01/08 12:32:48 spb Exp spb $
 *
 * Control program for the MD update
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define DECL
#include "coord.h"
#include "util.h"

//double second(void);

int main(int argc, char *argv[]) {

	int i,j,l;
	FILE *in, *out;
	double start,stop, tstart, tstop;
	double inputTime;
	double outputTime = 0;
	double computationTime = 0;
	double timing[9] = {0,0,0,0,0,0,0,0,0};
	char name[80];


	tstart = second();

	/*  timestep value */
	double dt = 0.02;

	/*  number of timesteps to use. */
	int Nstep = 100;
	int Nsave = 5;


	if (argc > 1) {
		Nstep = atoi(argv[1]);
	}

	P[Xcoord] = 0.0;
	P[Ycoord] = 0.0;
	P[Zcoord] = 8.0;

	/* set up multi dimensional arrays */
/*	r          = calloc(Nbody,sizeof(double));
	delta_r    = calloc(Nbody*Nbody,sizeof(double));
	mass       = calloc(Nbody,sizeof(double));
	visc       = calloc(Nbody,sizeof(double));
	f[0]       = calloc(Ndim*Nbody,sizeof(double));
	pos[0]     = calloc(Ndim*Nbody,sizeof(double));
	vel[0]     = calloc(Ndim*Nbody,sizeof(double));
	delta_x[0] = calloc(Ndim*Nbody*Nbody,sizeof(double));

	for (i = 1; i < Ndim; i++) {
		f[i]       = f[0] + i * Nbody;
		pos[i]     = pos[0] + i * Nbody;
		vel[i]     = vel[0] + i * Nbody;
		delta_x[i] = delta_x[0] + i*Nbody*Nbody;
	}
*/

	/* read the initial data from a file */
	start = second();

	collisions = 0;
	in = fopen("input.dat","r");

	if (!in) {
		perror("input.dat");
		exit(1);
	}

	for (i = 0; i < Nbody; i++) {
		fscanf(in,"%16le%16le%16le%16le%16le%16le%16le%16le\n",&mass[i], &visc[i],
				&pos[Xcoord][i], &pos[Ycoord][i], &pos[Zcoord][i],
				&vel[Xcoord][i], &vel[Ycoord][i], &vel[Zcoord][i]);
	}
	fclose(in);

	stop = second();

	inputTime = stop - start;

	for (i = 0; i < Ndim; i++) {
		for (j = 0; j < Nbody; j++) {
			f[i][j] = 0;
			r[j] = 0;
			for (l = 0; l < Nbody; l++) {
				delta_x[i][j*Nbody+l] = 0;
				delta_r[j*Nbody+l] = 0;
			}
		}
	}

	/*
	 * Run Nstep timesteps and time how long it took
	 */

	for (j = 1; j <= Nsave; j++) {
		start = second();

		evolve(Nstep, dt, timing); 

		stop = second();

		computationTime += stop - start;

		printf("%d timesteps took %f seconds\n",j*Nstep,stop-start);
		printf("collisions %d\n",collisions);

		/* write final result to a file */
		start = second();

		sprintf(name,"output.dat%03d",j*Nstep);
		out = fopen(name,"w");

		if (!out) {
			perror(name);
			exit(1);
		}

		for (i = 0; i < Nbody; i++) {
			fprintf(out,"%16.8E%16.8E%16.8E%16.8E%16.8E%16.8E%16.8E%16.8E\n",
					mass[i],visc[i],
					pos[Xcoord][i], pos[Ycoord][i], pos[Zcoord][i],
					vel[Xcoord][i], vel[Ycoord][i], vel[Zcoord][i]);
		}
		fclose(out);

		stop = second();
		outputTime += stop - start;
	}

	tstop = second();

	printf("\nTotal IO time = %f seconds\n", inputTime + outputTime);
	printf("Total computation time = %f seconds\n", computationTime);
	printf("Total program time = %f seconds\n\n", tstop - tstart);


	printf("Set viscocity:   %f seconds\n", timing[0]);
	printf("Add wind:   %f seconds\n", timing[1]);
	printf("Calculate distance from central mass:   %f secconds\n", timing[2]);
	printf("Calculate central force:   %f seconds\n", timing[3]);
	printf("Calculate pairwise separation:   %f seconds\n", timing[4]);
	printf("Calculate norm:   %f seconds\n", timing[5]);
	printf("Add pairwise forces:   %f seconds\n", timing[6]);
	printf("Update positions:   %f seconds\n", timing[7]);
	printf("Update velocities:   %f seconds\n", timing[8]);

}


/*
   double second()
   {
/* struct timeval { long        tv_sec; 
long        tv_usec;        };

struct timezone { int   tz_minuteswest;
int        tz_dsttime;      };     */

/*	struct timeval tp;
	struct timezone tzp;
	int i;

	i = gettimeofday(&tp,&tzp);
	return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
	}*/

