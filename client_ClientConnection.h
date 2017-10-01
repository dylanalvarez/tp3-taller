#ifndef TP3_TALLER_CLIENT_CONNECTION_H
#define TP3_TALLER_CLIENT_CONNECTION_H


#include "common_Connection.h"

#include <string>
#include <array>
#include <vector>

class ClientConnection : public Connection {
public:
  ClientConnection(std::string ip, std::string port);

  ClientConnection(const ClientConnection &) = delete;

  ClientConnection &operator=(const ClientConnection &) = delete;
};

#endif //TP3_TALLER_CLIENT_CONNECTION_H
