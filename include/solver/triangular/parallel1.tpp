#include "parallel1.hpp"

#include <vector>

#include <iostream>

#include "storage/matrix/sparse/csc.hpp"
#include "storage/vector/dense.hpp"

template <typename T>
storage::vector::Dense<T> solve(storage::matrix::sparse::CSC<T>& l, storage::vector::Dense<T>& b)
{
  storage::vector::Dense<T>& x = b;

  auto levelPtr = std::vector<int>(l.n() + 1, 0);
  auto levelIndex = std::vector<int>(l.n(), 0);
  auto indexMarked = std::vector<int>(l.n(), 0);
  auto marked = 0;
  auto levels = 0;

  for (; marked < l.n(); levels++)
  {
    levelPtr[levels] = marked;
    auto indexHasDependencies = std::vector<int>(l.n(), 0);
    for (auto j = 0; j < l.n(); j++)
    {
      if (indexMarked[j])
        continue;
      auto columnPointerJ = l.columnPointer(j);
      auto columnPointerNextJ = l.columnPointer(j + 1);
      for (auto p = columnPointerJ + 1; p < columnPointerNextJ; p++)
        indexHasDependencies[l.rowIndex(p)] = 1;
      if (indexHasDependencies[j])
        continue;
      indexMarked[j] = 1;
      levelIndex[marked] = j;
      marked++;
    }
  }
  levelPtr[levels] = marked;

  for (auto level = 0; level < levels; level++)
  {
//    #pragma omp parallel
    for (auto index = levelPtr[level]; index < levelPtr[level + 1]; index++)
    {
      auto j = levelIndex[index];

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
  }
  return x;
}
