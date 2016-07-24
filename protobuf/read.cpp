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

  std::fstream ifs;
  ifs.open(argv[1], std::ios::in | std::ios::binary);
  if (!person_list.ParseFromIstream(&ifs)) {
    std::cerr << "Failed to parse address book." << std::endl;
    return -1;
  }

  std::ofstream ofs;
  ofs.open("output.txt");
  
  for (int i = 0; i < person_list.person_size(); i++) {
    Person person = person_list.person(i);
    ofs << person.name () << ' ' << person.age ()  << '\n';
  }

  ofs.close();
  
  // Optional:  Delete all global objects allocated by libprotobuf.
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
