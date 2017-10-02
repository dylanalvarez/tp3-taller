#include "common_Connection.h"
#include "common_Exception.h"
#include "client_FileParser.h"
#include "client_ClientConnection.h"
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
  if (argc < 4) { throw Exception("Missing arguments"); }
  ClientConnection connection(argv[1], argv[2]);
  FileParser fileParser(argv[3]);
  std::string nextInstruction;
  while (!(nextInstruction = fileParser.parseNextInstruction()).empty()) {
    if (nextInstruction.find(" -> ") != std::string::npos) {
      std::cout << nextInstruction << std::endl;
    } else {
      connection.send(
        std::vector<char>(nextInstruction.begin(),
                          nextInstruction.end()));
      std::vector<char> responseOpcode = connection.receive(1);
      if (responseOpcode.empty()) { throw Exception("Server disconnected"); }
      size_t length = nextInstruction[0] == 'R' ? 10 : 20;
      if (responseOpcode[0] == 'E') { length = 5; }
      std::vector<char> responseContent = connection.receive(length);
      std::cout
        << responseOpcode[0]
        << std::string(responseContent.begin(), responseContent.end())
        << std::endl;
    }
  }
  return 0;
}
