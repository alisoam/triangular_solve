#ifndef STORAGE_VECTPR_DENSE_CSC_HPP
#define STORAGE_VECTPR_DENSE_CSC_HPP

#include <string>
#include <vector>

namespace storage::vector
{
  template <typename T = float>
  class Dense
  {
  public:
    Dense(const std::string& fileName);

    unsigned long long m() const;
    unsigned long long enteries() const;

    T value(unsigned long long index) const;
    void set(unsigned long long index, const T& value);

    void save(const std::string& fileName) const;
  private:
    void readMM(std::ifstream& file);
    std::string readMMHeader(std::ifstream& file);
    std::string readMMComments(std::ifstream& file);
    void readMMArray(std::ifstream& file, const std::string& size);
    void readMMCordinate(std::ifstream& file, const std::string& size);

    void writeMMHeader(std::ofstream& file) const;
    void writeMMComments(std::ofstream& file) const;
    void writeMMArray(std::ofstream& file) const;

    unsigned long long rows,
                       nonZeros;
    std::vector<T> values;
  };
  #include "dense.tpp"
}

#endif
