#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "common_Connection.h"
#include "server_SUBEManager.h"
#include "server_ServerConnectionFactory.h"
#include "server_ServerThread.h"
#include "server_WaitForQThread.h"
#include "AcceptFailedException.h"

void joinAndDelete(std::vector<ServerThread *> &threads,
                   WaitForQThread &waitForQThread) {
  std::for_each(threads.begin(), threads.end(),
                [](ServerThread *&serverThread) {
                  serverThread->join();
                  delete serverThread;
                });
  waitForQThread.join();
}

int main(int argc, char *argv[]) {
  ServerConnectionFactory connectionFactory("8080");
  SUBEManager subeManager;
  std::vector<ServerThread *> threads;
  WaitForQThread waitForQThread(connectionFactory);
  waitForQThread.start();

  while (connectionFactory.canAcceptConnection()) {
    try {
      threads.push_back(new ServerThread(connectionFactory, subeManager));
    } catch (AcceptFailedException &e) {
      joinAndDelete(threads, waitForQThread);
      return 0;
    }
    threads.back()->start();
  }

  joinAndDelete(threads, waitForQThread);
  return 0;
}
