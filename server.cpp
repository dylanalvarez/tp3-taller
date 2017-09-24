#include <iostream>
#include <vector>
#include <string>

#include "common_ConnectionHandler.h"

int main(int argc, char *argv[]) {
  ConnectionHandler connectionHandler("1024");
  std::vector<char> content = connectionHandler.receive(3 * 7);
  std::string message(content.begin(), content.end());
  std::cout << message << std::endl;
  return 0;
}
