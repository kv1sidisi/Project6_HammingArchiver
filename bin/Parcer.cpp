#include "Parcer.h"

bool IsThereAnyArguments(Arguments& arguments){
  return arguments.concatenate || arguments.del || arguments.append
  || arguments.extract || arguments.list || arguments.create;
}

void GetArchiveName(char* arg,Arguments& arguments){
  int i = 8;
  while (arg[i] != ']'){
    arguments.arch_name += arg[i];
    i++;
  }
}


bool Parcer(int argc, char** argv, Arguments& arguments){
  for(int i = 1; i < argc; i++){
    if (argv[i][0] == '-' && argv[i][1] != '-'){
      if (argv[i][1] == 'c'){
        arguments.create = true;
      } else if (argv[i][1] == 'f'){
        arguments.archname = true;
      } else if (argv[i][1] == 'l'){
        arguments.list = true;
      } else if (argv[i][1] == 'x'){
        arguments.extract = true;
      } else if (argv[i][1] == 'a'){
        arguments.append = true;
      } else if (argv[i][1] == 'd'){
        arguments.del = true;
      } else if (argv[i][1] == 'A'){
        arguments.concatenate = true;
      } else {
        return false;
      }
    } else if (argv[i][0] == '-' && argv[i][1] == '-'){
      if (argv[i][2] == 'c' && argv[i][3] == 'r'){
        arguments.create = true;
      } else if (argv[i][2] == 'f'){
        arguments.archname = true;
        arguments.got_archname = true;
        GetArchiveName(argv[i],arguments);
      } else if (argv[i][2] == 'l'){
        arguments.list = true;
      } else if (argv[i][2] == 'x'){
        arguments.extract = true;
      } else if (argv[i][2] == 'a'){
        arguments.append = true;
      } else if (argv[i][2] == 'd'){
        arguments.del = true;
      } else if (argv[i][2] == 'c' && argv[i][3] == 'o'){
        arguments.concatenate = true;
      } else {
        return false;
      }
    } else {
      if (IsThereAnyArguments(arguments)) {
        if (arguments.archname && !arguments.got_archname){
          arguments.got_archname = true;
          arguments.arch_name = argv[i];
        } else {
          arguments.filenames.push_back(argv[i]);
        }
      }
      else {
        return false;
      }
    }
  }
  return true;
}