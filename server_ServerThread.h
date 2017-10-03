#ifndef TP3_TALLER_SERVER_THREAD_H
#define TP3_TALLER_SERVER_THREAD_H


#include <thread>
#include <map>
#include "common_Connection.h"
#include "server_SUBEManager.h"
#include "common_Operation.h"
#include "common_Thread.h"
#include "server_ServerConnectionFactory.h"

/*
 * Recieves instructions from a client and updates subeManager accordingly
 */
class ServerThread : public Thread {
  public:
  /*
   * connectionFactory is used to instantiate a connection with a client
   */
  ServerThread(const ServerConnectionFactory &connectionFactory,
               SUBEManager &subeManager);

  ~ServerThread() override = default;

  protected:
  void run() override;

  private:
  const Connection connection;
  SUBEManager &subeManager;
  std::map<char, Operation> operations;
};


#endif //TP3_TALLER_SERVER_THREAD_H
