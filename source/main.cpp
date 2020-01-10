#include <chrono>
#include <functional>
#include <iostream>
#include <tuple>
#include <type_traits> 

#include "storage/matrix/sparse/csc.hpp"
#include "storage/vector/dense.hpp"
#include "solver/triangular/naive.hpp"
#include "solver/triangular/parallel1.hpp"
#include "solver/triangular/parallel2.hpp"

using namespace std;
using namespace chrono;

template<typename F>
auto responseTime(F function)
{
  static_assert(is_invocable<F>::value);
  auto start = chrono::system_clock::now();
  invoke(function);
  auto end = chrono::system_clock::now();
  return end - start;
}

void b_sparse(const string& baseAddress)
{
  cout << "b_sparse():" << endl;
  storage::matrix::sparse::CSC<double> l(baseAddress + "/af_0_k101.mtx");
  storage::vector::Dense<double> b(baseAddress + "/b_sparse_af_0_k101.mtx");

  auto l1 = l;
  auto b1 = b;
  unique_ptr<storage::vector::Dense<double>> x1;
  cout << "\tsolve Ax=B responseTime: "
            << duration_cast<milliseconds>(responseTime([&]()
                 {
                   auto x = new storage::vector::Dense(solver::triangular::naive::solve(l1, b1));
                   x1.reset(x);
                 }
               )).count()
            << "ms"
            << endl;
  x1->save(baseAddress + "/x_sparse_naive.mtx");

  auto l2 = l;
  auto b2 = b;
  unique_ptr<storage::vector::Dense<double>> x2;
  cout << "\tsolve Ax=B responseTime: "
            << chrono::duration_cast<chrono::milliseconds>(responseTime([&]()
                 {
                   auto x = new storage::vector::Dense(solver::triangular::parallel1::solve(l2, b2));
                   x2.reset(x);
                 }
               )).count()
            << "ms"
            << endl;
  x2->save(baseAddress + "/x_sparse_parallel1.mtx");

  auto l3 = l;
  auto b3 = b;
  unique_ptr<storage::vector::Dense<double>> x3;
  cout << "\tsolve Ax=B responseTime: "
            << chrono::duration_cast<chrono::milliseconds>(responseTime([&]()
                 {
                   auto x = new storage::vector::Dense(solver::triangular::parallel2::solve(l3, b3));
                   x3.reset(x);
                 }
               )).count()
            << "ms"
            << endl;
  x3->save(baseAddress + "/x_sparse_parallel2.mtx");
}

void b_dense(const string& baseAddress)
{
  cout << "b_dense():" << endl;
  storage::matrix::sparse::CSC<double> l(baseAddress + "/af_0_k101.mtx");
  storage::vector::Dense<double> b(baseAddress + "/b_dense_af_0_k101.mtx");

  auto l1 = l;
  auto b1 = b;
  unique_ptr<storage::vector::Dense<double>> x1;
  cout << "\tsolve Ax=B responseTime: "
            << duration_cast<milliseconds>(responseTime([&]()
                 {
                   auto x = new storage::vector::Dense(solver::triangular::naive::solve(l1, b1));
                   x1.reset(x);
                 }
               )).count()
            << "ms"
            << endl;
  x1->save(baseAddress + "/x_dense_naive.mtx");

  auto l2 = l;
  auto b2 = b;
  unique_ptr<storage::vector::Dense<double>> x2;
  cout << "\tsolve Ax=B responseTime: "
            << chrono::duration_cast<chrono::milliseconds>(responseTime([&]()
                 {
                   auto x = new storage::vector::Dense(solver::triangular::parallel1::solve(l2, b2));
                   x2.reset(x);
                 }
               )).count()
            << "ms"
            << endl;
  x2->save(baseAddress + "/x_dense_parallel1.mtx");

  auto l3 = l;
  auto b3 = b;
  unique_ptr<storage::vector::Dense<double>> x3;
  cout << "\tsolve Ax=B responseTime: "
            << chrono::duration_cast<chrono::milliseconds>(responseTime([&]()
                 {
                   auto x = new storage::vector::Dense(solver::triangular::parallel2::solve(l3, b3));
                   x3.reset(x);
                 }
               )).count()
            << "ms"
            << endl;
  x3->save(baseAddress + "/x_dense_parallel2.mtx");
}

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    cout << "base address should be passed as the first argument" << endl;
    return EXIT_FAILURE;
  }
  string baseAddress(argv[1]);
  b_sparse(baseAddress);
  b_dense(baseAddress);

  return EXIT_SUCCESS;
}

