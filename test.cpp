
#include "SYCL/sycl.hpp"
using namespace cl::sycl;

int main(void) {
    int N = 16;
 	int h_a[N];
 	int h_b[N];
 	int h_c[N];
    for (int i = 0; i < N; i++) {
        h_a[i] = h_b[i] = 1;
        h_c[i] = 0;
	}

	{       // SYCL region starts here
      queue myQueue;
      // Device buffers
      buffer<int, 1> d_a(h_a, N);
      buffer<int, 1> d_b(h_b, N);
      buffer<int, 1> d_c(h_c, N);

      command_group(myQueue, [&]() {
      	auto a = d_a.get_access<access::read>();
        auto b = d_b.get_access<access::read>();
        auto c = d_c.get_access<access::write>();
        
		parallel_for(nd_range<1>(range<1>(N)), kernel_functor<class vaddKernel>([=](item item)
        {
            int i = item.get_global_id(0);
            if (i < N) {
                c[i] = a[i] + b[i];
            }
        }));
      });
    }       // End of SYCL region
    
return 0;
}






auto sum = [] (int a, int b) { return a+b; }

class adderFunctor  {

	int k;

public:
    adderFunctor(int arg) : k(arg) {};

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
      )
      : ptr(p), a(A), b(B), c(C), size(N) {}

  
  void operator()(item item) {
    int i = item.get_global_id(0);
    
    if (i < size) {
      c[i] = a[i] + b[i];
    }
  }

};



command_group(myQueue, [&]() {
        auto a = d_a.get_access<access::read>();
        auto b = d_b.get_access<access::read>();
        auto c = d_c.get_access<access::read_write>();
        
        auto functor = vadd(a, b, c, N);
       
        parallel_for(nd_range<>(range<>(), range<>()),
          kernel_functor(functor));
        });




const char *kernel_src = R"EOK(
  __kernel void foo(__global int *k) {
  		int i = get_global_id(0);
  		k[i] = i;
  }
)EOK";


program foo_program(kernel_src, myQueue.get_context());

kernel *foo_kernel = foo_program.compile_kernel_by_name("foo");

    command_group(myQueue, [&]() {
    	auto buf = input_buffer.get_access<access::write>();
    	
    	pow_kernel->set_kernel_arg(buf);

    	parallel_for(nd_range<>(range<>()), foo_kernel);
    });
