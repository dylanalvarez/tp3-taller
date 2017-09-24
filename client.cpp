#include "common_ConnectionHandler.h"
#include "common_Exception.h"
#include "client_FileParser.h"
#include <vector>
#include <string>
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc < 2) { throw Exception("Missing file name"); }
  FileParser fileParser(argv[1]);
  std::cout << fileParser.parseNextInstruction();
  return 0;
}
