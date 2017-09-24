#ifndef TP3_TALLER_EXCEPTION_H
#define TP3_TALLER_EXCEPTION_H


#include <exception>
#include <string>

class Exception : public std::exception {
public:
  explicit Exception(std::string what);

  const char *what() const noexcept override;

private:
  std::string message;
};


#endif //TP3_TALLER_EXCEPTION_H
