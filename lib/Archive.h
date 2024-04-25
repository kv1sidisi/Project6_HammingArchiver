#pragma once
#include "Parcer.h"
#include <string>
#include <fstream>
#include <cmath>
#include <bitset>
#include <cstdio>
struct File {
  uint32_t info_bits;
  uint32_t encoded_file_size;
};

bool CreateArchive(const Arguments& arguments);

bool AddFile(const Arguments& arguments, const std::string& filename);

bool ExtractAll(const Arguments& arguments);

bool Extract(const Arguments& arguments,const std::string& file_to_extract);

bool Delete(const Arguments& arguments, std::string filename);

bool Concatenate(const Arguments& arguments);

bool List(const Arguments& arguments);