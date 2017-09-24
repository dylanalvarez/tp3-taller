#include <bitset>
#include <iostream>
#include <string>
#include "client_FileParser.h"
#include "common_Exception.h"

#define METADATA_LENGTH 2

FileParser::FileParser(std::string fileName) :
  source(std::ifstream(fileName, std::ios::binary)) {}

std::string FileParser::parseNextInstruction() {
  enum Operation {
    addAmount = 0,
    forceAddAmount = 1,
    checkAmount = 2,
    registerCard = 3,
    setAmount = 4
  };

  // read metadata
  unsigned char metadataBuffer[METADATA_LENGTH];
  source.read((char *) metadataBuffer, METADATA_LENGTH);
  if (!source) { throw Exception("Input file has wrong format"); }

  // extract Operation
  unsigned char opCode = (metadataBuffer[1] >> 3) & (unsigned char) 7;
  if (opCode > 4) { throw Exception("Wrong opcode"); }
//  auto operation = static_cast<Operation>(opCode);

  // for this to compile (temporary)
  return std::to_string(opCode);
}
