#ifndef TP3_TALLER_SERVER_SUBEMANAGER_H
#define TP3_TALLER_SERVER_SUBEMANAGER_H


#include <vector>
#include <map>
#include <mutex>
#include "common_Operation.h"

class SUBEManager {
public:
  char processInstruction(Operation, const std::vector<char> &id,
                          std::vector<char> &amount);

private:
  std::map<int32_t, int32_t> SUBEs;
  std::mutex mutex;
};


#endif //TP3_TALLER_SERVER_SUBEMANAGER_H
