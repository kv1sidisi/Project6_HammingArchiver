#pragma once
#include <iostream>
#include <vector>

struct Arguments {
  std::string arch_name;
  bool archname= false;
  bool got_archname= false;

  bool create = false;
  bool list = false;
  bool extract= false;
  bool append= false;
  bool del= false;
  bool concatenate= false;

  std::vector<std::string> filenames;
};

bool Parcer(int argc, char** argv, Arguments& arguments);

bool IsThereAnyArguments(Arguments& arguments);

void GetArchiveName(char* arg,Arguments& arguments);

