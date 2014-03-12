#include <math.h>

void visc_force(int N,double *f, double *visc, double *vel)
{
  int i;
          for(i=0;i<N;i++){
            f[i] = -visc[i] * vel[i];
          }
}
void wind_force(int N,double *f[3], double *visc, double *pos[3], double P[3])
{
  int i,j;
  double r;
          for(i=0;i<N;i++){
	    /* gas cloud rotating about axis with constant angular momentum per 
	     * particle, 
	     */
            r=0.0;
            for(j=0;j<3;j++){
	      r += pos[j][i] * pos[j][i];
	    }
	    f[0][i] = f[0][i] - ( visc[i] * pos[1][i] * P[2] )/r;
	    f[1][i] = f[1][i] + ( visc[i] * pos[2][i] * P[0] )/r;
	    f[2][i] = f[2][i] - ( visc[i] * pos[0][i] * P[1] )/r;
          }
}
void add_norm(int N,double *r, double *delta)
{
  int k;
        for(k=0;k<N;k++){
          r[k] += (delta[k] * delta[k]);
        }
}

double force(double W, double delta, double r){
  return W*delta/(pow(r,3.0));
}



