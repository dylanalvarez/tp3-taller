#define _POSIX_C_SOURCE 200112L

#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <utility>
#include <iostream>
#include <cstring>
#include <vector>
#include <string>
#include <fcntl.h>

#include "server_ServerConnectionFactory.h"
#include "AcceptFailedException.h"

ServerConnectionFactory::ServerConnectionFactory(std::string port) :
    skt(-1), wasManuallyShutDown(false) {
  struct addrinfo hints{};
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  struct addrinfo *result;
  int exitCode = getaddrinfo(nullptr, port.c_str(), &hints, &result);
  if (exitCode != 0) { throw Exception(gai_strerror(exitCode)); }

  for (struct addrinfo *ptr = result;
       ptr != nullptr;
       ptr = ptr->ai_next) {
    skt = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
  }

  if (skt == -1) {
    freeaddrinfo(result);
    throw Exception("Could not connect");
  }

  if (bind(skt, result->ai_addr, result->ai_addrlen) == -1) {
    freeaddrinfo(result);
    throw Exception(strerror(errno));
  }

  freeaddrinfo(result);

  if (listen(skt, 1) == -1) { throw Exception(strerror(errno)); }
}

bool ServerConnectionFactory::canAcceptConnection() {
  return !wasManuallyShutDown;
}

Connection ServerConnectionFactory::acceptConnection() const {
  try {
    int client_skt = accept(skt, nullptr, nullptr);
    if (client_skt == -1) throw AcceptFailedException();
    return Connection(client_skt);
  } catch (const std::runtime_error &error) {
    throw AcceptFailedException();
  }
}

void ServerConnectionFactory::shutdown() {
  ::shutdown(this->skt, SHUT_RDWR);
  close(this->skt);
  wasManuallyShutDown = true;
}

ServerConnectionFactory::~ServerConnectionFactory() {
  if (!wasManuallyShutDown) { shutdown(); }
}
