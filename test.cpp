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
