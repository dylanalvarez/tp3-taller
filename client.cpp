#include "common_ConnectionHandler.h"
#include "common_Exception.h"
#include "client_FileParser.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  if (argc < 2) { throw Exception("Missing file name"); }
  FileParser fileParser(argv[1]);
  std::string nextInstruction;
  while (!(nextInstruction = fileParser.parseNextInstruction()).empty()) {
    std::cout << nextInstruction << std::endl;
  }
  return 0;
}
