#ifndef STORAGE_MATRIX_SPARSE_CSC_HPP
#define STORAGE_MATRIX_SPARSE_CSC_HPP

#include <string>
#include <vector>

namespace storage::matrix::sparse
{
  template <typename T>
  class CSC
  {
  public:
    CSC(const std::string& fileName);

    unsigned long long n() const;
    unsigned long long m() const;
    unsigned long long enteries() const;

    unsigned long long columnPointer(unsigned long long inxdex) const;
    unsigned long long rowIndex(unsigned long long index) const;
    T value(unsigned long long index) const;
    void set(unsigned long long index, const T& value);

  private:
    void readMM(std::ifstream& file);
    void readMMHeader(std::ifstream& file);
    std::string readMMComments(std::ifstream& file);
    void readMMCordinate(std::ifstream& file, const std::string& size);

    unsigned long long rows,
                       columns,
                       nonZeros;
    std::vector<T> columnPointers;
    std::vector<T> rowIndexs;
    std::vector<T> values;
  };
}

#include "csc.tpp"

#endif
