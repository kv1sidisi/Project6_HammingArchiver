#include <iostream>
#include "Parcer.h"
#include "Archive.h"

int main(int argc, char** argv) {

  Arguments arguments;
  if (!Parcer(argc,argv,arguments)){
    return -6;
  }

  if (arguments.create){
    if (!CreateArchive(arguments)){
      return -2;
    }
  }

  if (arguments.append){
    if (!arguments.archname){
      std::cerr << "No archive name'\n'";
    }
    if(!AddFile(arguments,arguments.filenames[0]));
  }

  if (arguments.extract){
    if (!arguments.archname){
      std::cerr << "No archive name'\n'";
    }
    if (arguments.filenames.empty()){
      ExtractAll(arguments);
    }
    else{
      Extract(arguments,arguments.filenames[0]);
    }
  }

  if (arguments.del){
    if(!arguments.archname){
      std::cerr << "No archive name'\n'";
    }
    Delete(arguments,arguments.filenames[0]);
  }

  if (arguments.concatenate){
    if (arguments.filenames.size() != 2){
      std::cerr << "No archive names'\n'";
    }
    Concatenate(arguments);
  }

  if (arguments.list){
    if(!arguments.archname){
      std::cerr << "No archive names'\n'";
    }
    List(arguments);
  }
}