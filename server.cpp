#include <iostream>
#include <vector>

#include "common_Connection.h"
#include "server_SUBEManager.h"
#include "server_ServerConnectionFactory.h"
#include "server_ServerThread.h"

int main(int argc, char *argv[]) {
  ServerConnectionFactory connectionFactory("8080");
  SUBEManager subeManager;
  std::vector<ServerThread *> threads;

  Connection connection = connectionFactory.acceptConnection();
  ServerThread thread(connection, subeManager);
  threads.push_back(&thread);
  thread.start();

  std::for_each(threads.begin(), threads.end(),
                [](ServerThread *&serverThread) { serverThread->join(); });
  return 0;
}
