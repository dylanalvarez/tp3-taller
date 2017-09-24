#ifndef TP3_TALLER_FILE_PARSER_H
#define TP3_TALLER_FILE_PARSER_H

#include <fstream>
#include <string>
#include <bitset>

class FileParser {
public:
  explicit FileParser(std::string fileName);

  std::string parseNextInstruction();

private:
  std::ifstream source;
};


#endif //TP3_TALLER_FILE_PARSER_H
