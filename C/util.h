void visc_force(int N,double *f, double *visc, double *vel);
void add_norm(int N,double *r, double *delta);
double force(double W, double delta, double r);
void wind_force(int N,double f[][Nbody], double visc[], double pos[][Nbody], double P[]);
double second(void);
