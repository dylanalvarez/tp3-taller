#ifndef TP3_TALLER_CONNECTION_H
#define TP3_TALLER_CONNECTION_H

#include <string>
#include <array>
#include <vector>

class Connection {
public:
  explicit Connection(int fileDescriptor);

  void send(std::vector<char> content) const;

  std::vector<char> receive(size_t sizeInBytes) const;

  Connection &operator=(const Connection &) = delete;

  ~Connection();

protected:
  Connection() = default;

  int skt;
};

#endif //TP3_TALLER_CONNECTION_H
