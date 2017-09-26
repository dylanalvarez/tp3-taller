#include <bitset>
#include <iostream>
#include <cstring>
#include <string>
#include <netinet/in.h>
#include "client_FileParser.h"
#include "common_Exception.h"

#define METADATA_LENGTH 2
#define DATA_LENGTH 4
#define INVALID_TRANSACTION "E00001"

FileParser::FileParser(std::string fileName) :
  source(std::ifstream(fileName, std::ios::binary)) {}

std::string FileParser::parseNextInstruction() {
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
  if (idChecksum != idBuffer.count()) { return INVALID_TRANSACTION; }

  int32_t amount = 0;
  unsigned char amountChecksum = 0;
  if (operation != checkAmount && operation != registerCard) {
    // extract amount checksum from metadata
    amountChecksum = ((metadataBuffer[0] << 2) & (unsigned char) 28) |
                     ((metadataBuffer[1] >> 6) & (unsigned char) 3);

    // read amount
    std::bitset<DATA_LENGTH * 8> amountBuffer;
    if (!source.read((char *) &amountBuffer, DATA_LENGTH)) {
      throw Exception("Input file has wrong format");
    }
    if (amountChecksum != amountBuffer.count()) { return INVALID_TRANSACTION; }
    uint32_t unsignedAmount = ntohl(static_cast<uint32_t>(idBuffer.to_ulong()));
    std::memcpy(&amount, &unsignedAmount, 4);
  }

  return _toString(operation, id, amount);
}

std::string
FileParser::_toString(Operation operation, uint32_t id, int32_t amount) {
  std::string result;

  switch (operation) {
    case addAmount:
      result += "A";
      break;
    case forceAddAmount:
      result += "F";
      break;
    case checkAmount:
      result += "P";
      break;
    case registerCard:
      result += "R";
      break;
    case setAmount:
      result += "S";
  }
  result += _toFixedLengthString(id, 10);
  if (operation != checkAmount && operation != registerCard) {
    result += _toFixedLengthString(amount, 10);
  }
  return result;
}

std::string FileParser::_toFixedLengthString(int32_t number, size_t size) {
  if (number < 0) {
    return "-" + _toFixedLengthString((uint32_t) (-number), size - 1);
  } else {
    return _toFixedLengthString((uint32_t) number, size);
  }
}

std::string FileParser::_toFixedLengthString(uint32_t number, size_t size) {
  std::string numberAsString = std::to_string(number);
  std::string padding = std::string(size - numberAsString.length(), '0');
  return padding + numberAsString;
}
