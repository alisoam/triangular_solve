#include "naive.hpp"

#include <iostream>

#include "storage/matrix/sparse/csc.hpp"
#include "storage/vector/dense.hpp"

template <typename T>
storage::vector::Dense<T> solve(storage::matrix::sparse::CSC<T>& l, storage::vector::Dense<T>& b)
{
  storage::vector::Dense<T>& x = b;

  for (auto j = 0 ; j < l.n() ; j++)
  {
    auto columnPointerJ = l.columnPointer(j);
    auto columnPointerNextJ = l.columnPointer(j + 1);
    auto xJ = x.value(j) / l.value(columnPointerJ);
    x.set(j, xJ);
    for (auto p = columnPointerJ + 1; p < columnPointerNextJ; p++)
    {
      auto i = l.rowIndex(p);
      x.set(i, x.value(i) - l.value(p) * xJ);
    }
  }
  return x;
}