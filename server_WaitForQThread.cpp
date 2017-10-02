#include <iostream>
#include <string>
#include "server_WaitForQThread.h"

WaitForQThread::WaitForQThread(
    ServerConnectionFactory &serverConnectionFactory) :
    serverConnectionFactory(serverConnectionFactory) {}

void WaitForQThread::run() {
  std::string instruction;
  while (instruction != "q") {
    std::cin >> instruction;
  }
  serverConnectionFactory.shutdown();
}
