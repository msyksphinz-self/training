#include <iostream>
#include <fstream>
#include <string>
#include "Person.pb.h"

int main(int argc, char* argv[]) {
  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  if (argc != 2) {
    std::cerr << "Usage:  " << argv[0] << " PERSON_FILE" << std::endl;
    return -1;
  }

  PersonList person_list;

  std::ifstream ifs;
  ifs.open (argv[1]);
  
  while (!ifs.eof()) {
    std::string str_name;
    int32_t     age;
    ifs >> str_name;
    ifs >> age;

    Person *person = person_list.add_person();
    
    // std::cout << str_name << '\n';
    // std::cout << age << '\n';
    
    person->set_name (str_name);
    person->set_age (age);
  }

  std::ofstream ofs;
  ofs.open ("person.dat", std::ios::out | std::ios::trunc | std::ios::binary);
  if (!person_list.SerializeToOstream(&ofs)) {
    std::cerr << "Failed to write address book." << std::endl;
    return -1;
  }

  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}


