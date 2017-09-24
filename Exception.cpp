#include "Exception.h"

const char *Exception::what() const {
  return exception::what();
}
