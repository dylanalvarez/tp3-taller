#ifndef TP3_TALLER_SERVER_CONNECTION_FACTORY_H
#define TP3_TALLER_SERVER_CONNECTION_FACTORY_H


#include <string>
#include <array>
#include <vector>
#include "common_Connection.h"

class ServerConnectionFactory {
public:
  explicit ServerConnectionFactory(std::string port);

  Connection acceptConnection();

  ServerConnectionFactory(const ServerConnectionFactory &) = delete;

  ServerConnectionFactory &operator=(const ServerConnectionFactory &) = delete;

  ~ServerConnectionFactory();

private:
  int skt;
};


#endif //TP3_TALLER_SERVER_CONNECTION_FACTORY_H
