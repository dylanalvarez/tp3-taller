#define _POSIX_C_SOURCE 200112L

#include <sys/socket.h>
#include <unistd.h>
#include <utility>
#include <iostream>
#include <cstring>
#include <vector>
#include <string>

#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0 // for macOS compatibility
#endif

#include "common_Connection.h"
#include "common_Exception.h"

Connection::Connection(int fileDescriptor) : skt(fileDescriptor) {}

Connection::~Connection() {
  shutdown(this->skt, SHUT_RDWR);
  close(this->skt);
}

void Connection::send(std::vector<char> content) const {
  size_t sentBytes = 0;
  auto sizeInBytes = content.size();
  while (sentBytes < sizeInBytes) {
    ssize_t sent = ::send(this->skt,
                          &content[sentBytes],
                          sizeInBytes - sentBytes,
                          MSG_NOSIGNAL);
    if (sent <= 0) { throw Exception(strerror(errno)); }
    sentBytes += sent;
  }
}

std::vector<char> Connection::receive(size_t sizeInBytes) const {
  size_t receivedBytes = 0;
  std::vector<char> buffer(sizeInBytes);
  while (receivedBytes < sizeInBytes) {
    ssize_t received = recv(this->skt,
                            &buffer[receivedBytes],
                            sizeInBytes - receivedBytes,
                            MSG_NOSIGNAL);
    if (received < 0) {
      throw Exception(strerror(errno));
    } else if (received == 0) {
      return std::vector<char>();
    }
    receivedBytes += received;
  }
  return buffer;
}
