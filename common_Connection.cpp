#define _POSIX_C_SOURCE 200112L

#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <utility>
#include <iostream>
#include <cstring>
#include <vector>
#include <string>

#include "common_Connection.h"
#include "common_Exception.h"

Connection::Connection(std::string ip, std::string port) {
  bool isServer = (ip.empty());

  struct addrinfo hints{};
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  if (isServer) { hints.ai_flags = AI_PASSIVE; }

  struct addrinfo *result;
  int exitCode = getaddrinfo(
    isServer ? nullptr : ip.c_str(),
    port.c_str(),
    &hints,
    &result);
  if (exitCode != 0) { throw Exception(gai_strerror(exitCode)); }

  bool success = false;
  int skt = -1;

  for (struct addrinfo *ptr = result;
       ptr != nullptr && !success;
       ptr = ptr->ai_next) {
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
    int bindExitCode = bind(skt, result->ai_addr, result->ai_addrlen);
    int listenExitCode = listen(skt, 1);
    if (bindExitCode != -1 && listenExitCode != -1) {
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

Connection::Connection(std::string port) :
  Connection("", std::move(port)) {}

Connection::~Connection() {
  if (this->peer_skt != -1) {
    shutdown(this->peer_skt, SHUT_RDWR);
    close(this->peer_skt);
  }
  if (this->skt != -1) {
    shutdown(this->skt, SHUT_RDWR);
    close(this->skt);
  }
}

void Connection::send(std::vector<char> content) {
  size_t sentBytes = 0;
  auto sizeInBytes = content.size();
  while (sentBytes < sizeInBytes) {
    ssize_t sent = ::send(this->peer_skt,
                          &content[sentBytes],
                          sizeInBytes - sentBytes,
                          MSG_NOSIGNAL);
    if (sent <= 0) { throw Exception(strerror(errno)); }
    sentBytes += sent;
  }
}

std::vector<char> Connection::receive(size_t sizeInBytes) {
  size_t receivedBytes = 0;
  std::vector<char> buffer(sizeInBytes);
  while (receivedBytes < sizeInBytes) {
    ssize_t received = recv(this->peer_skt,
                            &buffer[receivedBytes],
                            sizeInBytes - receivedBytes,
                            MSG_NOSIGNAL);
    if (received <= 0) { throw Exception(strerror(errno)); }
    receivedBytes += received;
  }
  return buffer;
}
