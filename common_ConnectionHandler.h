#ifndef TP3_TALLER_CONNECTION_HANDLER_H
#define TP3_TALLER_CONNECTION_HANDLER_H

#include <string>
#include <array>
#include <vector>

class ConnectionHandler {
public:
  ConnectionHandler(std::string ip, std::string port);

  explicit ConnectionHandler(std::string port);

  void send(std::vector<char> content);

  std::vector<char> receive(size_t sizeInBytes);

  ConnectionHandler(const ConnectionHandler &) = delete;

  ConnectionHandler &operator=(const ConnectionHandler &) = delete;

  ~ConnectionHandler();

private:
  int skt;
  int peer_skt;
  std::string ip;
  std::string port;
};

#endif //TP3_TALLER_CONNECTION_HANDLER_H
