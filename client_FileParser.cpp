#include <bitset>
#include <iostream>
#include <cstring>
#include <string>
#include <netinet/in.h>
#include <map>
#include "client_FileParser.h"
#include "common_Exception.h"
#include "common_Utils.h"

#define METADATA_LENGTH 2
#define DATA_LENGTH 4

FileParser::FileParser(std::string fileName) :
  source(std::ifstream(fileName, std::ios::binary)) {}

std::string FileParser::parseNextInstruction() {
  bool invalidTransaction = false;
  // read metadata
  unsigned char metadataBuffer[METADATA_LENGTH];
  if (!source.read((char *) metadataBuffer, METADATA_LENGTH)) { return ""; }

  // extract opcode from metadata
  unsigned char opCode = (metadataBuffer[1] >> 3) & (unsigned char) 7;
  if (opCode > 4) { throw Exception("Wrong opcode"); }
  auto operation = static_cast<Operation>(opCode);

  // read id
  std::bitset<DATA_LENGTH * 8> idBuffer;
  if (!source.read((char *) &idBuffer, DATA_LENGTH)) {
    throw Exception("Input file has wrong format");
  }
  uint32_t id = ntohl(static_cast<uint32_t>(idBuffer.to_ulong()));

  // extract id checksum from metadata
  unsigned char idChecksum = (metadataBuffer[0] >> 3) & (unsigned char) 31;
  size_t idBitCount = idBuffer.count();
  if (idBitCount == 32) { idBitCount = 0; }
  if (idChecksum != idBitCount) { invalidTransaction = true; }

  int32_t amount = 0;
  unsigned char amountChecksum = 0;

  // extract amount checksum from metadata
  amountChecksum = ((metadataBuffer[0] << 2) & (unsigned char) 28) |
                   ((metadataBuffer[1] >> 6) & (unsigned char) 3);

  if (operation != checkAmount && operation != registerCard) {
    // read amount
    std::bitset<DATA_LENGTH * 8> amountBuffer;
    if (!source.read((char *) &amountBuffer, DATA_LENGTH)) {
      throw Exception("Input file has wrong format");
    }
    size_t amountBitCount = amountBuffer.count();
    if (amountBitCount == 32) { amountBitCount = 0; }
    if (amountChecksum != amountBitCount) { invalidTransaction = true; }
    uint32_t unsignedAmount = ntohl(
      static_cast<uint32_t>(amountBuffer.to_ulong()));
    std::memcpy(&amount, &unsignedAmount, 4);
  } else if (amountChecksum != 0) { invalidTransaction = true; }

  std::string result = _toString(operation, id, amount);
  if (invalidTransaction) { result += " -> E00001"; }
  return result;
}

std::string
FileParser::_toString(Operation operation, uint32_t id, int32_t amount) {
  std::string result;
  std::map<Operation, char> opCodes = {
    {addAmount,      'A'},
    {forceAddAmount, 'F'},
    {checkAmount,    'P'},
    {registerCard,   'R'},
    {setAmount,      'S'},
  };

  result += opCodes[operation] + Utils::toFixedLengthString(id, 10);
  if (operation != checkAmount && operation != registerCard) {
    result += Utils::toFixedLengthString(amount, 10);
  }
  return result;
}
