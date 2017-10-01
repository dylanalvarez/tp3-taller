#include <iostream>
#include <vector>
#include <map>
#include <string>

#include "common_Connection.h"
#include "common_Exception.h"
#include "common_Operation.h"
#include "server_SUBEManager.h"
#include "server_ServerConnectionFactory.h"

int main(int argc, char *argv[]) {
  ServerConnectionFactory connectionFactory("8081");
  Connection connection = connectionFactory.acceptConnection();
  SUBEManager subeManager;
  std::vector<char> content;
  std::map<char, Operation> operations = {
    {'A', addAmount},
    {'F', forceAddAmount},
    {'P', checkAmount},
    {'R', registerCard},
    {'S', setAmount},
  };
  while (!(content = connection.receive(11)).empty()) {
    std::vector<char> amount;
    if (operations.find(content[0]) == operations.end()) {
      throw Exception("Invalid opcode");
    }
    Operation operation = operations[content[0]];
    if (operation != checkAmount && operation != registerCard) {
      amount = connection.receive(10);
    }
    std::string request = std::string(content.begin(), content.end())
                          + std::string(amount.begin(), amount.end()) + " -> ";
    char errorCode = subeManager.processInstruction(
      operation,
      std::vector<char>(content.begin() + 1, content.end()),
      amount);
    if (errorCode == '0') {
      connection.send(content);
      connection.send(amount);
      std::cout << request << std::string(content.begin(), content.end())
                << std::string(amount.begin(), amount.end()) << std::endl;
    } else {
      std::vector<char> response{'E', '0', '0', '0', '0', errorCode};
      connection.send(response);
      std::cerr << request << std::string(response.begin(), response.end())
                << std::endl;
    }
  }
  return 0;
}
