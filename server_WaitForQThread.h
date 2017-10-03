#ifndef TP3_TALLER_WAITFORQTHREAD_H
#define TP3_TALLER_WAITFORQTHREAD_H

#include "server_ServerConnectionFactory.h"
#include "common_Thread.h"

/*
 * Waits for the user to enter 'q' in stdin, shutdown serverConnectionFactory
 * when that happens
 */
class WaitForQThread : public Thread {
  public:
  explicit WaitForQThread(ServerConnectionFactory &serverConnectionFactory);

  void run() override;

  private:
  ServerConnectionFactory &serverConnectionFactory;
};

#endif //TP3_TALLER_WAITFORQTHREAD_H
