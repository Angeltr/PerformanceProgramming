template <class T>
class vadd {
	accessor<T, 1, access::read, access::global_buffer> a;
	accessor<T, 1, access::read, access::global_buffer> b;
	accessor<T, 1, access::write, access::global_buffer> c;
	int size;
public:
	vadd(
	     accessor<T, 1, access::read, access::global_buffer> A,
	     accessor<T, 1, access::read, access::global_buffer> B,
	     accessor<T, 1, access::write, access::global_buffer> C,
	     int N
	    ) : a(A), b(B), c(C), size(N) {}

	void operator()(item item) {
		int i = item.get_global_id(0);
		if (i < size) {
			c[i] = a[i] + b[i];
		}
	}
}





{		// SYCL region starts here
	phi_selector selector;
  queue myQueue(selector);
        
  buffer<double, 1> d_a0(a0, sz*sz*sz);
  buffer<double, 1> d_a1(a1, sz*sz*sz);

  command_group (myQueue, [&]()
  {
  	auto a0 = d_a0.get_access<access::read_write>();
    auto a1 = d_a1.get_access<access::read_write>();

    for (iter = 0; iter < ITERATIONS; iter++) {

    	parallel_for(nd_range<3>(range<3>(n,n,n), range<3>(182,1,1)), kernel_functor<class kernel_compute>([=](item item)
    	{
    		int i = item.get_global_id(0) + 1;
      	int j = item.get_global_id(1) + 1;
      	int k = item.get_global_id(2) + 1;
      	int sz = item.get_global_size(0) + 2;

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
    	}));
            
    	parallel_for(nd_range<3>(range<3>(n,n,n), range<3>(182,1,1)), kernel_functor<class kernel_copy>([=](item item)
    	{
      	int i = item.get_global_id(0) + 1;
      	int j = item.get_global_id(1) + 1;
      	int k = item.get_global_id(2) + 1;
      	int sz = item.get_global_size(0) + 2;

      	a0[i*sz*sz+j*sz+k] = a1[i*sz*sz+j*sz+k];
			}));
  	}
  });     // Command group ends here
} // SYCL region ends here
