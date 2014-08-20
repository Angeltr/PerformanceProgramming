class phi_selector : public device_selector {
public:
        phi_selector() : device_selector() {}

        int operator()(const device &device) const {
                if (device.has_extension(strdup("cl_khr_spir"))) {
                        if (device.get_info<CL_DEVICE_TYPE>() == CL_DEVICE_TYPE_ACCELERATOR) {
                                return 1;
                        }
                }
                return -1;
        }
};





int i = get_global_id(0);
int j;

for (j = 0; j < 10000; j++) {           
        c[i] = a[i] + b[i];
}


class adderFunctor {
	int k;
public:
	adderFunctor(int kk) : k(kk) {};
	int operator()(int l) {
		return k+l;
	}
};
int main(void) {
	adderFunctor add5(5);
	cout << add5(5) << endl;
return 0;
}

int main(void) {
	vector<int> vec(10, 0);
	transform(vec.begin(), vec.end(), vec.begin(), adderFunctor(5));
return 0;
}


for (iter = 0; iter < ITERATIONS; iter++) {
#pragma acc parallel loop
      for (i = 1; i < n+1; i++) {
#pragma acc loop
        for (j = 1; j < n+1; j++) {
#pragma acc loop
          for (k = 1; k < n+1; k++) {
            a1[i*sz*sz+j*sz+k] = /* Sum of 27 neighbouring points. */
          }
        }
      }

#pragma acc parallel loop
      for (i = 1; i < n+1; i++) {
#pragma acc loop
        for (j = 1; j < n+1; j++) {
#pragma acc loop
          for (k = 1; k < n+1; k++) {
            a0[i*sz*sz+j*sz+k] = a1[i*sz*sz+j*sz+k];
          }
        }
      }
} /* end iteration loop */


double gosa, s0, ss;

for(n=0;n<nn;++n){
#pragma acc parallel loop private(i,j,k,s0,ss), reduction(+:gosa)
      for(i=1 ; i<imax-1 ; ++i){
	for(j=1 ; j<jmax-1 ; ++j){
	  for(k=1 ; k<kmax-1 ; ++k){
	    s0 = /* expression based on arrays a,b,c,wrk1,p. */
	       
	    ss = ( s0 * a[i][j][k][3] - p[i][j][k] ) * bnd[i][j][k];
	    gosa = gosa + ss*ss;
	    wrk2[i][j][k] = p[i][j][k] + omega * ss;
	  }
	}
      }
#pragma acc wait      
#pragma acc parallel loop
      for(i=1 ; i<imax-1 ; ++i)
	for(j=1 ; j<jmax-1 ; ++j)
	  for(k=1 ; k<kmax-1 ; ++k)
	    p[i][j][k] = wrk2[i][j][k];
} /* end n loop */



	cl_platform_id platform;
 	cl_device_id device;
    cl_context context;
    cl_command_queue command_queue;
    cl_program program;
    cl_kernel kernel_stencil, kernel_copy;
    cl_mem d_a0;            // input
    cl_mem d_a1;

    clGetPlatformIDs(1, &platform, &num_platforms);
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_ACCELERATOR, 1, &device, &num_devices);
	context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    command_queue = clCreateCommandQueue(context, device, 0, &err);

	fp = fopen("27stencil.cl", "r");
    source_str = (char *) malloc(MAX_SOURCE_SIZE);
    source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose(fp);

    program = clCreateProgramWithSource(context, 1, (const char **) &source_str, (const size_t *) &source_size, &err);
    err = clBuildProgram(program, 1, &device, "-I ../", NULL, NULL);

	kernel_stencil = clCreateKernel(program, "stencil", &err);
	kernel_copy = clCreateKernel(program, "copy", &err);
    
    d_a0 = clCreateBuffer(context, CL_MEM_READ_ONLY, bytes, NULL, &ret);
    d_a1 = clCreateBuffer(context, CL_MEM_WRITE_ONLY, bytes, NULL, &ret);
    
    clEnqueueWriteBuffer(command_queue, d_a0, CL_TRUE, 0, bytes, a0_init, 0, NULL, NULL);
    
    for (iter = 0; iter < ITERATIONS; iter++) {

        clSetKernelArg(kernel_compute, 0, bytes, &d_a0);
        clSetKernelArg(kernel_compute, 1, bytes, &d_a1);
        clSetKernelArg(kernel_compute, 2, sizeof(double), &fac);
               
		size_t global[3] = { n, n, n };

        ret = clEnqueueNDRangeKernel(command_queue, kernel_compute, 3, NULL, global, NULL, 0, NULL, NULL);
        clFinish(command_queue);

		clSetKernelArg(kernel_copy, 0, bytes, &d_a0);
        clSetKernelArg(kernel_copy, 1, bytes, &d_a1);

        clEnqueueNDRangeKernel(command_queue, kernel_copy, 3, NULL, global, NULL, 0, NULL, NULL);
        clFinish(command_queue);
    } // end iteration loop

	ret = clEnqueueReadBuffer(command_queue, d_a0, CL_TRUE, 0, bytes, a1, 0, NULL, NULL);
	
    /* Free OpenCL resources */
    clReleaseMemObject(d_a0);
    clReleaseMemObject(d_a1);
    clFlush(command_queue);
    clFinish(command_queue);
    clReleaseKernel(kernel_compute);
    clReleaseKernel(kernel_copy);
    clReleaseProgram(program);
    clReleaseCommandQueue(command_queue);
    clReleaseContext(context);



__kernel void stencil(__global double *in, __global double *out, double fac) {

  int i = get_global_id(0)+1;
  int j = get_global_id(1)+1;
  int k = get_global_id(2)+1;
  int sz = get_global_size(0)+2;

  out[i * sz * sz+j*sz+k] = (
        in[i*sz*sz+(j-1)*sz+k] + in[i*sz*sz+(j+1)*sz+k] +
        in[(i-1) * sz * sz+j*sz+k] + in[(i+1) * sz * sz+j*sz+k] +
        in[(i-1)* sz * sz+(j-1) * sz+k] + in[(i-1) * sz * sz+(j+1)*sz+k] +
        in[(i+1) * sz * sz+(j-1) * sz+k] + in[(i+1) * sz * sz+(j+1)*sz+k] +
        in[i * sz * sz+(j-1) * sz+(k-1)] + in[i * sz * sz+(j+1) * sz+(k-1)] +
        in[(i-1) * sz * sz+j * sz+(k-1)] + in[(i+1) * sz * sz+j* sz+(k-1)] +
        in[(i-1) * sz * sz+(j-1) * sz+(k-1)] + in[(i-1) * sz * sz+(j+1) * sz+(k-1)] +
        in[(i+1)*sz*sz+(j-1)*sz+(k-1)] + in[(i+1)*sz*sz+(j+1)*sz+(k-1)] +
        in[i*sz*sz+(j-1)*sz+(k+1)] + in[i*sz*sz+(j+1)*sz+(k+1)] +
        in[(i-1)*sz*sz+j*sz+(k+1)] + in[(i+1)*sz*sz+j*sz+(k+1)] +
        in[(i-1)*sz*sz+(j-1)*sz+(k+1)] + in[(i-1)*sz*sz+(j+1)*sz+(k+1)] +
        in[(i+1)*sz*sz+(j-1)*sz+(k+1)] + in[(i+1)*sz*sz+(j+1)*sz+(k+1)] +
        in[i*sz*sz+j*sz+(k-1)] + in[i*sz*sz+j*sz+(k+1)]
        ) * fac;
}

__kernel void copy(__global double *a0, __global double *a1) {

  int i = get_global_id(0)+1;
  int j = get_global_id(1)+1;
  int k = get_global_id(2)+1;
  int sz = get_global_size(0)+2;

  a0[i*sz*sz+j*sz+k] = a1[i*sz*sz+j*sz+k];
}
