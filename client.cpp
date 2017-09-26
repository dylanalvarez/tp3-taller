#include "common_ConnectionHandler.h"
#include "common_Exception.h"
#include "client_FileParser.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  if (argc < 2) { throw Exception("Missing file name"); }
  ConnectionHandler connectionHandler("127.0.0.1", "8080");
  FileParser fileParser(argv[1]);
  std::string nextInstruction;
  while (!(nextInstruction = fileParser.parseNextInstruction()).empty()) {
    connectionHandler.send(
      std::vector<char>(nextInstruction.begin(),
                        nextInstruction.end()));
  }
  connectionHandler.send(std::vector<char>(' '));
  return 0;
}
