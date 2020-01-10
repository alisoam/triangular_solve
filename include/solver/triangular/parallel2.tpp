#include "parallel2.hpp"

#include <memory>
#include <iostream>

#include "storage/matrix/sparse/csc.hpp"
#include "storage/vector/dense.hpp"

template <typename T>
static void mark(int* xFilter, const storage::matrix::sparse::CSC<T>& l, unsigned long long index)
{
  if (xFilter[index])
    return;
  xFilter[index] = 1;
  for (auto p = l.columnPointer(index) + 1; p < l.columnPointer(index + 1); p++)
    if (l.value(p) != 0)
      mark(xFilter, l, l.rowIndex(p));
}

template <typename T>
storage::vector::Dense<T> solve(storage::matrix::sparse::CSC<T>& l, storage::vector::Dense<T>& b)
{
  storage::vector::Dense<T>& x = b;
  auto xFilter = std::make_unique<int[]>(l.n());

  memset(xFilter.get(), 0, sizeof(int) * l.n());

  #pragma omp parallel for
  for (auto i = 0; i < l.n(); i++)
    if (b.value(i) != 0)
      mark(xFilter.get(), l, i);

  #pragma omp parallel for
  for (auto j = 0; j < l.n(); j++)
  {
    if (!xFilter[j])
      continue;
    for (auto p = l.columnPointer(j) + 1; p < l.columnPointer(j + 1); p++)
      l.set(p, l.value(p) / l.value(l.columnPointer(l.rowIndex(p))));
  }

  #pragma omp parallel for
  for (auto j = 0; j < l.n(); j++)
  {
    if (!xFilter[j])
      continue;
    auto columnPointerJ = l.columnPointer(j);
    x.set(j, x.value(j) / l.value(columnPointerJ));
    l.set(columnPointerJ, 1);
  }

  #pragma omp parallel for
  for (auto j = 0 ; j < l.n() ; j++)
  {
    if (!xFilter[j])
      continue;
    auto columnPointerJ = l.columnPointer(j);
    auto columnPointerNextJ = l.columnPointer(j + 1);
    auto xJ = x.value(j);
    for (auto p = columnPointerJ + 1; p < columnPointerNextJ; p++)
    {
      auto rowIndex = l.rowIndex(p);
      x.set(rowIndex, x.value(rowIndex) - l.value(p) * xJ);
    }
  }
  return x;
}
