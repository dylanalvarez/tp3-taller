#ifndef TP3_TALLER_FILE_PARSER_H
#define TP3_TALLER_FILE_PARSER_H

#include <fstream>
#include <string>
#include <bitset>

#include "common_Operation.h"

/*
 * Takes filename relative to executable and translates each instruction to
 * one that can be sent to server
 */
class FileParser {
public:
  explicit FileParser(std::string fileName);

  /*
   * If there are not any more instructions, an empty string is returned
   */
  std::string parseNextInstruction();

  private:
  std::string _toString(Operation operation, uint32_t id, int32_t amount);

  std::ifstream source;
};


#endif //TP3_TALLER_FILE_PARSER_H
