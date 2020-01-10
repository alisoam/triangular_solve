#include "csc.hpp"

#include <numeric>
#include <fstream>
#include <regex>
#include <sstream>
#include <stdexcept>

namespace storage::matrix::sparse
{
  template <typename T>
  unsigned long long CSC<T>::enteries() const
  {
    return nonZeros;
  }

  template <typename T>
  unsigned long long CSC<T>::m() const
  {
    return rows;
  }

  template <typename T>
  unsigned long long CSC<T>::n() const
  {
    return columns;
  }

  template <typename T>
  unsigned long long CSC<T>::columnPointer(unsigned long long index) const
  {
    return columnPointers[index];
  }

  template <typename T>
  unsigned long long CSC<T>::rowIndex(unsigned long long index) const
  {
    return rowIndexs[index];
  }

  template <typename T>
  T CSC<T>::value(unsigned long long index) const
  {
    return values[index];
  }

  template <typename T>
  void CSC<T>::set(unsigned long long index, const T& value)
  {
    values[index] = value;
  }

  template <typename T>
  CSC<T>::CSC(const std::string& fileName)
  {
    auto file = std::ifstream();
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    file.open(fileName);

    std::smatch base_match;
    if (std::regex_match(fileName, base_match, std::regex(R"_(.*\.mtx$)_")))
      readMM(file);
    else
      throw std::runtime_error("matrix format not supported");
  }

  template <typename T>
  void CSC<T>::readMM(std::ifstream& file) {
    readMMHeader(file);
    auto size = readMMComments(file);
    readMMCordinate(file, size);
  }

  template <typename T>
  void CSC<T>::readMMHeader(std::ifstream& file)
  {
    std::string header;
    getline(file, header);
    auto headerStream = std::stringstream(header);
    headerStream.exceptions(std::ios::badbit | std::ios::failbit);
    std::string fileType, object, format, field, symetric;
    headerStream >> fileType >> object >> format >> field >> symetric;
    if (format == "coordinate")
      return;
    throw std::runtime_error("matrix format not found");
  }

  template <typename T>
  std::string CSC<T>::readMMComments(std::ifstream& file)
  {
    while (true)
    {
      std::string line;
      getline(file, line);
      if (line[0] != '%') 
        return line;
    }
  }

  template <typename T>
  void CSC<T>::readMMCordinate(std::ifstream& file, const std::string& size)
  {
    auto sizeStream = std::stringstream(size);
    sizeStream.exceptions(std::ios::badbit | std::ios::failbit);
    sizeStream >> rows >> columns >> nonZeros;

    columnPointers = std::vector<T>(columns + 1);
    rowIndexs = std::vector<T>(nonZeros);
    values = std::vector<T>(nonZeros);

    unsigned long long lastColumn = 0;
    columnPointers.at(0) = lastColumn;

    for (auto index = 0; index < nonZeros; index++)
    {
      std::string line;
      getline(file, line);

      auto valueStream = std::stringstream(line);
      valueStream.exceptions(std::ios::badbit | std::ios::failbit);
      unsigned long long i, j;
      T value;
      valueStream >> i >> j >> value;
      i--;
      j--;

      while (lastColumn != j)
      {
        lastColumn++;
        columnPointers.at(lastColumn) = index;
      }
      rowIndexs.at(index) = i;
      values.at(index) = value;
    }

    while (lastColumn != columns)
    {
      lastColumn++;
      columnPointers.at(lastColumn) = columnPointers.at(lastColumn - 1);
    }
  }
}
