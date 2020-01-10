#ifndef SOLVER_TRIANGULAR_PARALLEL1_HPP
#define SOLVER_TRIANGULAR_PARALLEL1_HPP

#include <string>
#include <vector>

#include <omp.h>

namespace solver::triangular::parallel1
{
  template <typename L, typename B>
  B solve(L l, B b);

  #include "parallel1.tpp"
}



#endif
