#include "common_ConnectionHandler.h"
#include "common_Exception.h"
#include "client_FileParser.h"
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
  if (argc < 2) { throw Exception("Missing file name"); }
  ConnectionHandler connectionHandler("127.0.0.1", "8080");
  FileParser fileParser(argv[1]);
  std::string nextInstruction;
  while (!(nextInstruction = fileParser.parseNextInstruction()).empty()) {
    if (nextInstruction[0] == 'E') {
      std::cout << nextInstruction << std::endl;
    } else {
      connectionHandler.send(
        std::vector<char>(nextInstruction.begin(),
                          nextInstruction.end()));
      std::vector<char> responseOpcode = connectionHandler.receive(1);
      size_t length = nextInstruction[0] == 'R' ? 10 : 20;
      if (responseOpcode[0] == 'E') {
        length = 5;
        std::cout << nextInstruction << " -> ";
      }
      std::vector<char> responseContent = connectionHandler.receive(length);
      std::cout
        << responseOpcode[0]
        << std::string(responseContent.begin(), responseContent.end())
        << std::endl;
    }
  }
  connectionHandler.send(std::vector<char>(11, ' '));
  return 0;
}
