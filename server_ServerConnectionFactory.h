#ifndef TP3_TALLER_SERVER_CONNECTION_FACTORY_H
#define TP3_TALLER_SERVER_CONNECTION_FACTORY_H


#include <string>
#include <array>
#include <vector>
#include "common_Connection.h"

/*
 * Creates a socket, binds and listens
 */
class ServerConnectionFactory {
public:
  explicit ServerConnectionFactory(std::string port);

  /*
   * Waits for a client to try to connect, generates a Connection when that
   * happens. If it's shutdown() in the middle, it throws an
   * AcceptFailedException
   */
  Connection acceptConnection() const;

  /*
   * Checks if the Connection Factory has been shutdown()
   */
  bool canAcceptConnection();

  /*
   * Interrupts current acceptConnection and makes this instance useless
   * (canAcceptConnection will return false from now on)
   */
  void shutdown();

  ServerConnectionFactory(const ServerConnectionFactory &) = delete;

  ServerConnectionFactory &operator=(const ServerConnectionFactory &) = delete;

  ~ServerConnectionFactory();

  private:
  int skt;
  bool wasManuallyShutDown;
};


#endif //TP3_TALLER_SERVER_CONNECTION_FACTORY_H
