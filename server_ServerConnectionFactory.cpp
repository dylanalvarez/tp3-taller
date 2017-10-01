#define _POSIX_C_SOURCE 200112L

#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <utility>
#include <iostream>
#include <cstring>
#include <vector>
#include <string>

#include "server_ServerConnectionFactory.h"
#include "common_Exception.h"

ServerConnectionFactory::ServerConnectionFactory(std::string port) : skt(-1) {
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

Connection ServerConnectionFactory::acceptConnection() {
  int client_skt = accept(skt, nullptr, nullptr);
  if (client_skt == -1) throw Exception("Accepted invalid socket");
  return Connection(client_skt);
}

ServerConnectionFactory::~ServerConnectionFactory() {
  shutdown(this->skt, SHUT_RDWR);
  close(this->skt);
}
