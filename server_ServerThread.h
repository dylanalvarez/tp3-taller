#ifndef TP3_TALLER_SERVER_THREAD_H
#define TP3_TALLER_SERVER_THREAD_H


#include <thread>
#include <map>
#include "common_Connection.h"
#include "server_SUBEManager.h"
#include "common_Operation.h"
#include "common_Thread.h"
#include "server_ServerConnectionFactory.h"

class ServerThread : public Thread {
  public:
  ServerThread(const ServerConnectionFactory &connectionFactory,
               SUBEManager &subeManager);

  ServerThread(ServerThread &&other) noexcept;

  ~ServerThread() override = default;

  protected:
  void run() override;

  private:
  const Connection connection;
  SUBEManager &subeManager;
  std::map<char, Operation> operations;
};


#endif //TP3_TALLER_SERVER_THREAD_H
