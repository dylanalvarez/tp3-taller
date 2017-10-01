#ifndef TP3_TALLER_THREAD_H
#define TP3_TALLER_THREAD_H


#include <thread>
#include <map>
#include "common_Connection.h"
#include "server_SUBEManager.h"
#include "common_Operation.h"

class ServerThread {
public:
  ServerThread(const Connection &connection, SUBEManager &subeManager);

  void start();

  void join();

  ServerThread(const ServerThread &) = delete;

  ServerThread &operator=(const ServerThread &) = delete;

  ServerThread(ServerThread &&other) noexcept;

  virtual ~ServerThread() = default;

protected:
  virtual void run();

private:
  std::thread thread;
  const Connection &connection;
  SUBEManager &subeManager;
  std::map<char, Operation> operations;
};


#endif //TP3_TALLER_THREAD_H
