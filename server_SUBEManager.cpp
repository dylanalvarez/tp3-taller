#include "server_SUBEManager.h"

char
SUBEManager::processInstruction(Operation operation, long id, long amount) {
  switch (operation) {
    case addAmount:
      if (SUBEs.find(id) == SUBEs.end()) { return 2; }
      if (SUBEs[id] + amount < 0) { return 3; }
      SUBEs[id] += amount;
      break;
    case forceAddAmount:
      break;
    case checkAmount:
      break;
    case registerCard:
      break;
    case setAmount:
  }
  return 0;
}
