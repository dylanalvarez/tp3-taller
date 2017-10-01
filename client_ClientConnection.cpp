#define _POSIX_C_SOURCE 200112L

#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <utility>
#include <iostream>
#include <cstring>
#include <vector>
#include <string>

#include "common_Exception.h"
#include "client_ClientConnection.h"

ClientConnection::ClientConnection(std::string ip, std::string port) {
  struct addrinfo hints{};
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  struct addrinfo *result;
  int exitCode = getaddrinfo(ip.c_str(), port.c_str(), &hints, &result);
  if (exitCode != 0) throw Exception(gai_strerror(exitCode));

  bool success = false;
  for (struct addrinfo *ptr = result;
       ptr != nullptr && !success;
       ptr = ptr->ai_next) {
    skt = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (skt != -1) {
      exitCode = connect(skt, ptr->ai_addr, ptr->ai_addrlen);
      if (exitCode != -1)
        success = true;
      else
        close(skt);
    }
  }

  freeaddrinfo(result);
  if (!success) throw Exception(strerror(errno));
}
