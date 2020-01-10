#ifndef SOLVER_TRIANGULAR_NAIVE_HPP
#define SOLVER_TRIANGULAR_NAIVE_HPP

#include <string>
#include <vector>

namespace solver::triangular::naive
{
  template <typename L, typename B>
  B solve(L l, B b);

  #include "naive.tpp"
}



#endif
