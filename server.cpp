#include <iostream>
#include "common_ConnectionHandler.h"

#define SUCCESS 0

int main(int argc, char *argv[]) {
  ConnectionHandler connectionHandler("127.0.0.1", "80");
  std::vector<char> content = connectionHandler.recieve(3 * 7);
  std::string message(content.begin(), content.end());
  std::cout << message << std::endl;
  return SUCCESS;
}
