#include "Archive.h"
#include "Hamming.h"

bool CreateArchive(const Arguments& arguments) {
  std::fstream file("../ArchiveFiles/"+arguments.arch_name, std::ios::app);

  if (!file.is_open()) {
    std::cerr << "Archive wasn't created\n";
    return false;
  } else {
    if (arguments.filenames.empty()) {
      std::cout << "Archive created successfully\n";
      return true;
    } else {
      for (int i = 0; i < arguments.filenames.size(); i++) {
        if (!AddFile(arguments, arguments.filenames[i])) {
          std::cerr << "File wasn't added to archive\n";
          return false;
        };
      }
      return true;
    }
  }
}

bool AddFile(const Arguments& arguments, const std::string& filename) {
  std::fstream arch("../ArchiveFiles/"+arguments.arch_name, std::ios::app | std::ios::binary);
  if (!arch.is_open()) {
    std::cerr << "Archive wasn't opened\n";
    return false;
  }

  std::ifstream file(filename, std::ifstream::binary | std::ifstream::ate);
  if (!file.is_open()) {
    std::cerr << "File wasn't opened\n";
    return false;
  }

  File file_size;
  file_size.info_bits = file.tellg() * 8;
  file.seekg(0, std::ios_base::beg);
  file_size.encoded_file_size = file_size.info_bits * 1.5;

  arch << filename << '\n';
  arch << std::to_string(file_size.encoded_file_size) << '\n';

  for(int i = 0; i < file_size.info_bits/8;i++){
    std::string encoded;
    encoded = std::bitset<8>(file.get()).to_string();
    encoded = Encoder(encoded);
    arch << encoded;
  }
  arch << std::endl;
  file.close();
  arch.close();
  return true;
}

bool ExtractAll(const Arguments& arguments) {
  std::ifstream arch("../ArchiveFiles/"+arguments.arch_name, std::ifstream::binary | std::ifstream::ate);
  if (!arch.is_open()) {
    std::cerr << "Archive wasn't opened\n";
    return false;
  }

  uint64_t arch_size = arch.tellg();
  arch.seekg(0, std::ios_base::beg);
  uint64_t pos = arch.tellg();

  while (pos < arch_size){
    std::string file_size;
    uint64_t int_file_size;
    std::string file_name;
    std::getline(arch, file_name);
    std::getline(arch, file_size);
    int_file_size = std::stoull(file_size);

    std::fstream file(file_name, std::ios::app | std::ios::binary);
    if (!file.is_open()) {
      std::cerr << "File wasn't created\n";
      return false;
    }

    for(int i = 0; i < int_file_size/12; i++){
      std::string to_decode;
      for(int k = 0; k < 12; k++){
        to_decode += arch.get();
      }
      std::bitset<8> symb(Decoder(to_decode));
      to_decode = static_cast<char>(symb.to_ulong());
      file<<to_decode;
    }
    arch.seekg(1, std::ios::cur);
    pos = arch.tellg();
  }
  return true;
}

bool Extract(const Arguments& arguments, const std::string& file_to_extract) {
  std::ifstream arch("../ArchiveFiles/"+arguments.arch_name, std::ifstream::binary | std::ifstream::ate);
  if (!arch.is_open()) {
    std::cerr << "Archive wasn't opened\n";
    return false;
  }

  uint64_t arch_size = arch.tellg();
  arch.seekg(0, std::ios_base::beg);
  uint64_t pos = arch.tellg();
  bool found = false;
  while (pos < arch_size) {
    std::string file_size;
    uint64_t int_file_size;
    std::string file_name;
    std::getline(arch, file_name);
    std::getline(arch, file_size);
    int_file_size = std::stoull(file_size);
    if (file_name == file_to_extract) {
      std::fstream file(file_name, std::ios::app | std::ios::binary);
      if (!file.is_open()) {
        std::cerr << "File wasn't created\n";
        return false;
      }

      for(int i = 0; i < int_file_size/12; i++){
        std::string to_decode;
        for(int k = 0; k < 12; k++){
          to_decode += arch.get();
        }
        std::bitset<8> symb(Decoder(to_decode));
        to_decode = static_cast<char>(symb.to_ulong());
        file<<to_decode;
        found = true;
      }
    } else {
      arch.seekg(int_file_size, std::ios::cur);
    }
    arch.seekg(1, std::ios::cur);
    pos = arch.tellg();
  }
  if (found){
    arch.close();
    Delete(arguments,file_to_extract);
    return true;
  }
  std::cerr << "File to extract wasn't found";
  return false;
}

bool Delete(const Arguments& arguments, std::string file_to_delete){
  std::ifstream arch("../ArchiveFiles/"+arguments.arch_name, std::ifstream::binary);
  if (!arch.is_open()) {
    std::cerr << "Archive wasn't opened\n";
    return false;
  }
  std::ofstream file("../ArchiveFiles/temp.haf", std::ios::binary);
  if (!file.is_open()) {
    std::cerr << "TempArchive wasn't created\n";
    return false;
  }

  bool deleted = false;
  arch.seekg(0, std::ios_base::end);
  uint64_t arch_size = arch.tellg();
  arch.seekg(0, std::ios_base::beg);
  uint64_t pos = arch.tellg();

  while (pos < arch_size) {
    std::string file_size;
    uint64_t int_file_size;
    std::string file_name;
    std::getline(arch, file_name);
    std::getline(arch, file_size);
    int_file_size = std::stoull(file_size);

    if (file_name != file_to_delete) {
      file << file_name << std::endl;
      file << file_size << std::endl;
      for(int i = 0; i < int_file_size; i++){
        char ch = arch.get();
        file << ch;
      }
      file << std::endl;
    } else {
      deleted = true;
      arch.seekg(int_file_size, std::ios::cur);
    }
    arch.seekg(1, std::ios::cur);
    pos = arch.tellg();
  }
  if (!deleted){
    std::cerr << "File to delete wasn't found";
    return false;
  }
  arch.close();
  file.close();

  std::string path = "../ArchiveFiles/arc.haf";
  if (std::remove(path.c_str()) != 0) {
    std::cerr << "Error deleting file: " << "../arc.haf" << std::endl;
  }

  if (rename("../ArchiveFiles/temp.haf", ("../ArchiveFiles/"+arguments.arch_name).c_str()) != 0) {
    std::cerr << "Error renaming file: " << "../arc.haf" << std::endl;
  }

  return true;
}

bool Concatenate(const Arguments& arguments){
  std::fstream file("../ArchiveFiles/"+arguments.arch_name, std::ios::app);

  if (!file.is_open()) {
    std::cerr << "Archive wasn't created\n";
    return false;
  }
  file.close();

  AddFile(arguments,arguments.filenames[0]);
  std::remove((arguments.filenames[0]).c_str());
  AddFile(arguments,arguments.filenames[1]);
  std::remove((arguments.filenames[1]).c_str());

}

bool List(const Arguments& arguments){
  std::ifstream arch("../ArchiveFiles/"+arguments.arch_name, std::ifstream::binary | std::ifstream::ate);
  if (!arch.is_open()) {
    std::cerr << "Archive wasn't opened\n";
    return false;
  }

  uint64_t arch_size = arch.tellg();
  arch.seekg(0, std::ios_base::beg);
  uint64_t pos = arch.tellg();

  while (pos < arch_size){
    std::string file_size;
    uint64_t int_file_size;
    std::string file_name;
    std::getline(arch, file_name);
    std::getline(arch, file_size);
    int_file_size = std::stoull(file_size);
    std::cout << file_name << " ";
    arch.seekg(1 + int_file_size, std::ios::cur);
    pos = arch.tellg();
  }
  return true;
}