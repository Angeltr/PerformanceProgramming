class vadd {
 
 accessor<int, 1, access::read, access::global_buffer> a;
 accessor<int, 1, access::read, access::global_buffer> b;
 accessor<int, 1, access::write, access::global_buffer> c;
 
 int size;

 public:
  
  vadd(
       accessor<int, 1, access::read_write, access::global_buffer> A,
       accessor<int, 1, access::read_write, access::global_buffer> B,
       accessor<int, 1, access::read_write, access::global_buffer> C,
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
