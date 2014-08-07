class example_functor {
 /* ptr is an accessor to a global buffer. 
  * The buffer itself will be assigned when creating the class.
  */
 accessor<int, 1, access::read_write, access::global_buffer> ptr;
 /* random_num is a normal attribute for the functor, that will
  * be initialized with a local value on the construction of the 
  * functor
  */
 int random_num;

 public:
  /* The constructor of the functor takes the accessor as a parameter.
   */
  example_functor(
      accessor<int, 1, access::read_write, access::global_buffer> p)
      : ptr(p) {
        /* Generate a random number between 1 and 100.
         * This is executed on the host.
         */
        random_num = std::rand()%(100-1)+1;
      }

  /* The overload of the operator is used to execute the kernel
   */
  void operator()(item item) {
    ptr[item.get_global()] = random_num;
  }

  /* Kernel functors are normal C++ classes, therefore, they can have
   * other methods
   */
  int get_random() {
    return random_num;
  }
};
