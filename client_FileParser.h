#ifndef TP3_TALLER_FILE_PARSER_H
#define TP3_TALLER_FILE_PARSER_H

#include <fstream>
#include <string>
#include <bitset>

#include "common_Operation.cpp"

class FileParser {
public:
  explicit FileParser(std::string fileName);

  std::string parseNextInstruction();

private:
  std::string _toString(Operation operation, uint32_t id, int32_t amount);

  std::string _toFixedLengthString(int32_t, size_t size);

  std::string _toFixedLengthString(uint32_t, size_t size);

  std::ifstream source;
};


#endif //TP3_TALLER_FILE_PARSER_H
