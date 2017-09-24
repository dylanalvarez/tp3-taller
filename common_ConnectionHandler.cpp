#define _POSIX_C_SOURCE 200112L

#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <unistd.h>
#include <cerrno>
#include <utility>
#include <iostream>

#include "common_ConnectionHandler.h"
#include "common_Exception.h"

ConnectionHandler::ConnectionHandler(std::string ip, std::string port) :
  ip(std::move(ip)), port(std::move(port)) {
  bool isServer = (this->ip.empty());

  struct addrinfo hints{};
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  if (isServer) {
    hints.ai_flags = AI_PASSIVE;
  } else {
    hints.ai_flags = 0;
  }

  this->skt = -1;
  this->peer_skt = -1;

  struct addrinfo *result;
  int exitCode = getaddrinfo(
    isServer ? nullptr : this->ip.c_str(),
    this->port.c_str(),
    &hints,
    &result);
  if (exitCode != 0) { throw Exception(gai_strerror(exitCode)); }

  bool success = false;
  int skt = -1;

  for (struct addrinfo *ptr = result;
       ptr != nullptr && !success; ptr = ptr->ai_next) {
    skt = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (skt != -1 && !isServer) {
      exitCode = connect(skt, ptr->ai_addr, ptr->ai_addrlen);
      if (exitCode != -1) {
        success = true;
      } else {
        close(skt);
      }
    }
  }

  if (isServer) {
    this->skt = skt;
    success = false;
    int a = bind(skt, result->ai_addr, result->ai_addrlen);
    std::cout << strerror(errno);
    int b = listen(skt, 1);
    std::cout << strerror(errno);

    if (a != -1 && b != -1) {
      int client_skt = accept(skt, nullptr, nullptr);
      if (client_skt != -1) {
        success = true;
        this->peer_skt = client_skt;
      }
    }
    if (!success) {
      close(skt);
    }
  } else {
    this->peer_skt = skt;
  }

  freeaddrinfo(result);

  if (!success) {
    throw Exception("Could not connect");
  }
}

ConnectionHandler::ConnectionHandler(std::string port) :
  ConnectionHandler("", std::move(port)) {}

ConnectionHandler::~ConnectionHandler() {
  if (this->peer_skt != -1) {
    shutdown(this->peer_skt, SHUT_RDWR);
    close(this->peer_skt);
  }
  if (this->skt != -1) {
    shutdown(this->skt, SHUT_RDWR);
    close(this->skt);
  }
}

void ConnectionHandler::send(std::vector<char> content) {
  unsigned long sentBytes = 0;
  auto sizeInBytes = content.size();
  while (sentBytes < sizeInBytes) {
    auto sent = (int) ::send(this->peer_skt, &content[sentBytes],
                             (size_t) (sizeInBytes - sentBytes),
                             MSG_NOSIGNAL);
    if (sent <= 0) {
      throw Exception(strerror(errno));
    }
    sentBytes += sent;
  }
}

std::vector<char> ConnectionHandler::recieve(unsigned long sizeInBytes) {
  unsigned long receivedBytes = 0;
  std::vector<char> buffer(sizeInBytes);
  while (receivedBytes < sizeInBytes) {
    auto received = (int) recv(this->peer_skt,
                               &buffer[receivedBytes],
                               (size_t) (sizeInBytes -
                                         receivedBytes),
                               MSG_NOSIGNAL);
    if (received <= 0) {
      throw Exception(strerror(errno));
    }
    receivedBytes += received;
  }
  return buffer;
}
