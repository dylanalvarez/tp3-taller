#ifndef TP2_TALLER_EXCEPTION_H
#define TP2_TALLER_EXCEPTION_H


#include <exception>

class Exception : public std::exception {
public:
  Exception(std::string what);

  const char *what() const override;

  Exception(const Exception &) = delete;

  Exception &operator=(const Exception &) = delete;
};


#endif //TP2_TALLER_EXCEPTION_H
