#ifndef TP3_TALLER_SERVER_SUBEMANAGER_H
#define TP3_TALLER_SERVER_SUBEMANAGER_H


#include <vector>
#include <map>
#include "common_Operation.cpp"

class SUBEManager {
public:
  char processInstruction(Operation, long id, long amount);

private:
  std::map<long, long> SUBEs;
};


#endif //TP3_TALLER_SERVER_SUBEMANAGER_H
