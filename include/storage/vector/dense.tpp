#include "dense.hpp"

#include <numeric>
#include <fstream>
#include <regex>
#include <sstream>
#include <stdexcept>

template <typename T>
Dense<T>::Dense(const std::string& fileName)
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
unsigned long long Dense<T>::m() const
{
  return rows;
}

template <typename T>
T Dense<T>::value(unsigned long long index) const
{
  return values[index];
}

template <typename T>
void Dense<T>::set(unsigned long long index, const T& value)
{
  values[index] = value;
}

template <typename T>
void Dense<T>::readMM(std::ifstream& file) {
  auto format = readMMHeader(file);
  auto size = readMMComments(file);
  if (format == "array")
    readMMArray(file, size);
  else
    readMMCordinate(file, size);
}

template <typename T>
std::string Dense<T>::readMMHeader(std::ifstream& file)
{
  std::string header;
  getline(file, header);
  auto headerStream = std::stringstream(header);
  headerStream.exceptions(std::ios::badbit | std::ios::failbit);
  std::string fileType, object, format, field, symetric;
  headerStream >> fileType >> object >> format >> field >> symetric;
  if (format == "array" || format == "coordinate")
    return format;
  throw std::runtime_error("matrix format not found");
}

template <typename T>
std::string Dense<T>::readMMComments(std::ifstream& file)
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
void Dense<T>::readMMArray(std::ifstream& file, const std::string& size)
{
  auto sizeStream = std::stringstream(size);
  sizeStream.exceptions(std::ios::badbit | std::ios::failbit);
  unsigned long long columns;
  sizeStream >> rows >> columns;
  if (columns != 1)
    throw std::runtime_error("matrix is not a vector");

  values = std::vector<T>(rows, 0);

  for (auto index = 0; index < rows; index++)
  {
    std::string line;
    getline(file, line);

    auto valueStream = std::stringstream(line);
    valueStream.exceptions(std::ios::badbit | std::ios::failbit);
    T value;
    valueStream >> value;

    values.at(index) = value;
  }
}

template <typename T>
void Dense<T>::readMMCordinate(std::ifstream& file, const std::string& size)
{
  auto sizeStream = std::stringstream(size);
  sizeStream.exceptions(std::ios::badbit | std::ios::failbit);
  unsigned long long columns, nonZero;
  sizeStream >> rows >> columns >> nonZeros;

  values = std::vector<T>(rows, 0);

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
    values.at(i) = value;
  }
}

template <typename T>
void Dense<T>::save(const std::string& fileName) const
{
  auto file = std::ofstream();
  file.exceptions(std::ofstream::failbit | std::ofstream::badbit);
  file.open(fileName);

  writeMMHeader(file);
  writeMMComments(file);
  writeMMArray(file);
}

template <typename T>
void Dense<T>::writeMMHeader(std::ofstream& file) const
{
  file << "%%MatrixMarket matrix array real general" << std::endl;
}

template <typename T>
void Dense<T>::writeMMComments(std::ofstream& file) const
{
  file << "% Here should be the comments" << std::endl;
}

template <typename T>
void Dense<T>::writeMMArray(std::ofstream& file) const
{
  file << rows << " " << 1 << std::endl;

  for (auto index = 0; index < rows; index++)
    file << values.at(index) << std::endl;
}
