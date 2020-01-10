#ifndef SOLVER_TRIANGULAR_PARALLEL2_HPP
#define SOLVER_TRIANGULAR_PARALLEL2_HPP

#include <string>
#include <vector>

#include <omp.h>

namespace solver::triangular::parallel2
{
  template <typename L, typename B>
  B solve(L l, const B b);

  #include "parallel2.tpp"
}

#endif
