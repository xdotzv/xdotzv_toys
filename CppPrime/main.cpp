#include <iostream>

template<unsigned > struct Prime;
template<unsigned, unsigned, unsigned> struct Sieve;

template <unsigned n, unsigned k>
struct Divisible{
  enum { value = (n % Prime<k>::value == 0) || Divisible<n, k-1>::value};
};

template <unsigned n>
struct Divisible<n, 0>{
  enum { value = 0};
};

template<unsigned div, unsigned n, unsigned k>
struct Sieve
{
  enum { value = n };
};

template<unsigned n, unsigned k>
struct Sieve<1, n, k>{
  enum { value = Sieve<Divisible<n+1, k>::value, n+1, k>::value };
};


template<unsigned n>
struct Prime{

   enum { value = Sieve<1, Prime<n-1>::value, n-1>::value };
  // enum { value = Sieve<Divisible<Prime<n-1>::value+1, n-1>::value, Prime<n-1>::value+1, n-1>::value};
};

template <>
struct Prime<1>{
  enum { value = 2};
};

int main()
{
  std::cout << Prime<2>::value << std::endl;
  std::cout << Prime<3>::value << std::endl;
  std::cout << Prime<4>::value << std::endl;
  std::cout << Prime<5>::value << std::endl;
  std::cout << Prime<6>::value << std::endl;
  std::cout << Prime<200>::value << std::endl;

  return 0;
}
