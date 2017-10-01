#include <iostream>
#include <vector>

#include "common_Connection.h"
#include "server_SUBEManager.h"
#include "server_ServerConnectionFactory.h"
#include "server_ServerThread.h"

int main(int argc, char *argv[]) {
  ServerConnectionFactory connectionFactory("8081");
  Connection connection = connectionFactory.acceptConnection();
  SUBEManager subeManager;
  std::vector<ServerThread *> threads;
  ServerThread thread(connection, subeManager);
  threads.push_back(&thread);
  thread.start();
  thread.join();
  return 0;
}
