#ifndef TP3_TALLER_COMMON_UTILS_H
#define TP3_TALLER_COMMON_UTILS_H


#include <cstdint>
#include <cstddef>
#include <string>

namespace Utils {
  std::string toFixedLengthString(int32_t, size_t size);

  std::string toFixedLengthString(uint32_t, size_t size);
}


#endif //TP3_TALLER_COMMON_UTILS_H
