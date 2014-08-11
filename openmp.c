for (iter = 0; iter < ITERATIONS; iter++) {

	#pragma omp parallel for default(none) private(i,j,k) shared(a0, a1, fac, n, sz) collapse(3)
	for (i = 1; i < n+1; i++) {
  	  for (j = 1; j < n+1; j++) {
  			  for (k = 1; k < n+1; k++) {
        	  	a1[i*sz*sz+j*sz+k] = (
                  a0[i*sz*sz+(j-1)*sz+k] + a0[i*sz*sz+(j+1)*sz+k] +
                  a0[(i-1)*sz*sz+j*sz+k] + a0[(i+1)*sz*sz+j*sz+k] +
                  a0[(i-1)*sz*sz+(j-1)*sz+k] + a0[(i-1)*sz*sz+(j+1)*sz+k] +
                  a0[(i+1)*sz*sz+(j-1)*sz+k] + a0[(i+1)*sz*sz+(j+1)*sz+k] +
                  a0[i*sz*sz+(j-1)*sz+(k-1)] + a0[i*sz*sz+(j+1)*sz+(k-1)] +
                  a0[(i-1)*sz*sz+j*sz+(k-1)] + a0[(i+1)*sz*sz+j*sz+(k-1)] +
                  a0[(i-1)*sz*sz+(j-1)*sz+(k-1)] + a0[(i-1)*sz*sz+(j+1)*sz+(k-1)] +
                  a0[(i+1)*sz*sz+(j-1)*sz+(k-1)] + a0[(i+1)*sz*sz+(j+1)*sz+(k-1)] +
                  a0[i*sz*sz+(j-1)*sz+(k+1)] + a0[i*sz*sz+(j+1)*sz+(k+1)] +
                  a0[(i-1)*sz*sz+j*sz+(k+1)] + a0[(i+1)*sz*sz+j*sz+(k+1)] +
                  a0[(i-1)*sz*sz+(j-1)*sz+(k+1)] + a0[(i-1)*sz*sz+(j+1)*sz+(k+1)] +
                  a0[(i+1)*sz*sz+(j-1)*sz+(k+1)] + a0[(i+1)*sz*sz+(j+1)*sz+(k+1)] +
                  a0[i*sz*sz+j*sz+(k-1)] + a0[i*sz*sz+j*sz+(k+1)]
                  ) * fac;
       		}
    	}
	}

	#pragma omp parallel for default(none) private(i,j,k) shared(a0, a1, n, sz) collapse(3)
	for (i = 1; i < n+1; i++) {
  	  for (j = 1; j < n+1; j++) {
    	    for (k = 1; k < n+1; k++) {
      	      a0[i*sz*sz+j*sz+k] = a1[i*sz*sz+j*sz+k];
        	}
    	}
	}

} /* end iteration loop */
