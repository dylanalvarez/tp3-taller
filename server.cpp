#include <iostream>
#include <vector>

#include "common_ConnectionHandler.h"
#include "common_Exception.h"
#include "common_Operation.cpp"
#include "server_SUBEManager.h"

int main(int argc, char *argv[]) {
  ConnectionHandler connectionHandler("8080");
  SUBEManager subeManager;
  std::vector<char> content;
  while ((content = connectionHandler.receive(11)) != std::vector<char>(' ')) {
    Operation operation;
    std::vector<char> amount;
    switch (content[0]) {
      case 'A':
        operation = addAmount;
        break;
      case 'F':
        operation = forceAddAmount;
        break;
      case 'P':
        operation = checkAmount;
        break;
      case 'R':
        operation = registerCard;
        break;
      case 'S':
        operation = setAmount;
        break;
      default:
        throw Exception("Invalid opcode");
    }
    if (operation != checkAmount && operation != registerCard) {
      amount = connectionHandler.receive(10);
    }
    char errorCode = subeManager.processInstruction(
      operation,
      std::stoi(std::string(content.begin() + 1, content.end())),
      std::stoi(std::string(amount.begin(), amount.end())));
    if (errorCode == 0) {
      connectionHandler.send(content);
      connectionHandler.send(amount);
    } else {
      connectionHandler.send(
        std::vector<char>{'E', '0', '0', '0', '0', errorCode});
    }
  }
  return 0;
}
