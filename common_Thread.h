#ifndef TP3_TALLER_THREAD_H
#define TP3_TALLER_THREAD_H

#include <thread>

class Thread {
  public:
  void start();

  void join();

  Thread(Thread &&other) noexcept;

  Thread(const Thread &) = delete;

  Thread &operator=(const Thread &) = delete;

  Thread() = default;

  virtual ~Thread() = default;

  protected:
  virtual void run() = 0;

  private:
  std::thread thread;
};


#endif //TP3_TALLER_THREAD_H
