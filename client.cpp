#include "common_ConnectionHandler.h"

#define SUCCESS 0

int main(int argc, char *argv[]) {
  ConnectionHandler connectionHandler("127.0.0.1", "80");
  std::string message = "message";
  connectionHandler.send(std::vector<char>(message.begin(), message.end()));
  connectionHandler.send(std::vector<char>(message.begin(), message.end()));
  connectionHandler.send(std::vector<char>(message.begin(), message.end()));
  return SUCCESS;
}
