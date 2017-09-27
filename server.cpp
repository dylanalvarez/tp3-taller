#include <iostream>
#include <vector>
#include <map>

#include "common_ConnectionHandler.h"
#include "common_Exception.h"
#include "common_Operation.h"
#include "server_SUBEManager.h"

int main(int argc, char *argv[]) {
  ConnectionHandler connectionHandler("8080");
  SUBEManager subeManager;
  std::vector<char> content;
  std::map<char, Operation> operations = {
    {'A', addAmount},
    {'F', forceAddAmount},
    {'P', checkAmount},
    {'R', registerCard},
    {'S', setAmount},
  };
  while (
    (content = connectionHandler.receive(11)) != std::vector<char>(11, ' ')) {
    std::vector<char> amount;
    if (operations.find(content[0]) == operations.end()) {
      throw Exception("Invalid opcode");
    }
    Operation operation = operations[content[0]];
    if (operation != checkAmount && operation != registerCard) {
      amount = connectionHandler.receive(10);
    }
    char errorCode = subeManager.processInstruction(
      operation,
      std::vector<char>(content.begin() + 1, content.end()),
      amount);
    if (errorCode == '0') {
      connectionHandler.send(content);
      connectionHandler.send(amount);
    } else {
      connectionHandler.send(
        std::vector<char>{'E', '0', '0', '0', '0', errorCode});
    }
  }
  return 0;
}
