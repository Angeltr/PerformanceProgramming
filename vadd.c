

	cl_device_id device_id;
	cl_context context;
	cl_command_queue command_queue;
	cl_mem d_a, d_b, d_c;
	cl_program program;
	cl_kernel kernel;
	cl_platform_id platform_id;

	FILE *fp;
	char *source_str;
	size_t source_size;

	fp = fopen("hello.cl", "r");
	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);

	clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ACCELERATOR, 1, &device_id, &ret_num_devices);

	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);
	clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	kernel = clCreateKernel(program, "vadd", &ret);

	d_a = clCreateBuffer(context, CL_MEM_READ_ONLY, N*sizeof(int), NULL, &ret);
	d_b = clCreateBuffer(context, CL_MEM_READ_ONLY, N*sizeof(int), NULL, &ret);
	d_c = clCreateBuffer(context, CL_MEM_WRITE_ONLY, N*sizeof(int), NULL, &ret);

	clEnqueueWriteBuffer(command_queue, d_a, CL_TRUE, 0, N*sizeof(int), a, 0, NULL, NULL);
	clEnqueueWriteBuffer(command_queue, d_b, CL_TRUE, 0, N*sizeof(int), b, 0, NULL, NULL);

	clSetKernelArg(kernel, 0, N*sizeof(int), &d_a);
	clSetKernelArg(kernel, 1, N*sizeof(int), &d_b);
	clSetKernelArg(kernel, 2, N*sizeof(int), &d_c);
	clSetKernelArg(kernel, 3, sizeof(int), &N);

	size_t global = N;
	clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global, NULL, 0, NULL, NULL);
	clFinish(command_queue);

	ret = clEnqueueReadBuffer(command_queue, d_c, CL_TRUE, 0, N * sizeof(int), h_c, 0, NULL, NULL);

	clFlush(command_queue);
	clFinish(command_queue);
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseMemObject(memobj);
	clReleaseCommandQueue(command_queue);
	clReleaseContext(context);
