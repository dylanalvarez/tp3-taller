#include <iostream>
#include <string>
#include <vector>
#include "server_ServerThread.h"
#include "common_Exception.h"


ServerThread::ServerThread(const Connection &connection,
                           SUBEManager &subeManager) :
    connection(connection),
    subeManager(subeManager),
    operations{{'A', addAmount},
               {'F', forceAddAmount},
               {'P', checkAmount},
               {'R', registerCard},
               {'S', setAmount}} {}

void ServerThread::start() {
  thread = std::thread(&ServerThread::run, this);
}

void ServerThread::join() {
  thread.join();
}

ServerThread::ServerThread(ServerThread &&other) noexcept :
    ServerThread(other.connection, other.subeManager) {
  this->thread = std::move(other.thread);
}

void ServerThread::run() {
  std::vector<char> content;
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
}

