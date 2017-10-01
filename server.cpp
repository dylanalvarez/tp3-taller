#include <iostream>
#include <vector>

#include "common_Connection.h"
#include "server_SUBEManager.h"
#include "server_ServerConnectionFactory.h"
#include "server_ServerThread.h"

int main(int argc, char *argv[]) {
  ServerConnectionFactory connectionFactory("8081");
  SUBEManager subeManager;

  ServerThread thread(connectionFactory.acceptConnection(), subeManager);
  thread.start();
  thread.join();
  return 0;
}
