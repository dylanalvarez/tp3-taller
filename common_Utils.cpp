#include "common_Utils.h"
#include <string>

std::string Utils::toFixedLengthString(int32_t number, size_t size) {
  if (number < 0) {
    return "-" + toFixedLengthString((uint32_t) (-number), size - 1);
  } else {
    return toFixedLengthString((uint32_t) number, size);
  }
}

std::string Utils::toFixedLengthString(uint32_t number, size_t size) {
  std::string numberAsString = std::to_string(number);
  std::string padding = std::string(size - numberAsString.length(), '0');
  return padding + numberAsString;
}
