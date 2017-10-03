#include "server_SUBEManager.h"
#include "common_Utils.h"
#include <vector>
#include <string>

char
SUBEManager::processInstruction(Operation operation,
                                const std::vector<char> &idVector,
                                std::vector<char> &amountVector) {
  uint32_t id = static_cast<uint32_t>(std::stoll(
      std::string(idVector.begin(), idVector.end())));
  int32_t amount = amountVector.empty() ? 0 : std::stoi(
      std::string(amountVector.begin(), amountVector.end()));

  bool operationNeedsExistingId = operation != registerCard;
  {
    std::lock_guard<std::mutex> lock(mutex);
    bool idWasFound = SUBEs.find(id) != SUBEs.end();
    if (operationNeedsExistingId && !idWasFound) { return '2'; }
    if (!operationNeedsExistingId && idWasFound) { return '4'; }

    switch (operation) {
      case registerCard:
        SUBEs[id] = 0;
        break;
      case addAmount:
        if (SUBEs[id] + amount < 0 && amount < 0) { return '3'; }
      case forceAddAmount:
        SUBEs[id] += amount;
        break;
      case setAmount:
        SUBEs[id] = amount;
      case checkAmount:
        break;
    }

    std::string amountString = Utils::toFixedLengthString(SUBEs[id], 10);
    if (operation != registerCard) {
      amountVector = std::vector<char>(amountString.begin(),
                                       amountString.end());
    }
  }
  return '0';
}
