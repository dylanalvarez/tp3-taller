#ifndef TP3_TALLER_CONNECTION_HANDLER_H
#define TP3_TALLER_CONNECTION_HANDLER_H

#include <string>
#include <array>
#include <vector>

class Connection {
public:
  Connection(std::string ip, std::string port);

  explicit Connection(std::string port);

  void send(std::vector<char> content);

  std::vector<char> receive(size_t sizeInBytes);

  Connection(const Connection &) = delete;

  Connection &operator=(const Connection &) = delete;

  ~Connection();

private:
  int skt;
  int peer_skt;
};

#endif //TP3_TALLER_CONNECTION_HANDLER_H
