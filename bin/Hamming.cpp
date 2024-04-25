#include "Hamming.h"

uint32_t ParityBitsNum(uint32_t info_bits) {
  uint32_t r = 1;
  while ((1 << r) < (info_bits + r + 1)) {
    r++;
  }
  return r;
}

inline bool charToBool(char c) {
  return c == '1';
}

std::string Decoder(std::string input){
  int index_sum = 0;
  std::string result;

  if (charToBool(input[0]) != (charToBool(input[2]) ^ charToBool(input[4]) ^ charToBool(input[6]) ^ charToBool(input[8]) ^ charToBool(input[10]))) {
    index_sum += 1;
  }
  if (charToBool(input[1]) != (charToBool(input[2]) ^ charToBool(input[5]) ^ charToBool(input[6]) ^ charToBool(input[9]) ^ charToBool(input[10]))) {
    index_sum += 2;
  }
  if (charToBool(input[3]) != (charToBool(input[4]) ^ charToBool(input[5]) ^ charToBool(input[6]) ^ charToBool(input[11]))) {
    index_sum += 4;
  }
  if (charToBool(input[7]) != (charToBool(input[8]) ^ charToBool(input[9]) ^ charToBool(input[10]) ^ charToBool(input[11]))) {
    index_sum += 8;
  }

  if (index_sum != 0){
    input[index_sum - 1] = static_cast<char>(!(static_cast<bool>(input[index_sum - 1])));
  }

  for (size_t i = 2; i < input.size(); i++) {
    if (i != 3 && i != 7) {
      result += input[i];
    }
  }
  return result;
}


std::string Encoder(std::string input){
  std::string result = "000000000000";

  result[2] = input[0];
  result[4] = input[1];
  result[5] = input[2];
  result[6] = input[3];
  result[8] = input[4];
  result[9] = input[5];
  result[10] = input[6];
  result[11] = input[7];

  result[0] = (charToBool(result[2]) ^ charToBool(result[4]) ^ charToBool(result[6]) ^ charToBool(result[8]) ^ charToBool(result[10])) ? '1' : '0';
  result[1] = (charToBool(result[2]) ^ charToBool(result[5]) ^ charToBool(result[6]) ^ charToBool(result[9]) ^ charToBool(result[10])) ? '1' : '0';
  result[3] = (charToBool(result[4])^ charToBool(result[5]) ^ charToBool(result[6]) ^ charToBool(result[11])) ? '1' : '0';
  result[7] = (charToBool(result[8]) ^ charToBool(result[9]) ^ charToBool(result[10]) ^ charToBool(result[11])) ? '1' : '0';

  return result;
}

