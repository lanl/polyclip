
#include <iostream>
#include "../gmv/io.h"
int main(int argc, char* argv[]) {
  if(argc < 3) {
    std::cout << "USAGE: compare_outputs [FILE_1] [FILE_2]\n";
  }
  if(polyclip::io::compare_files(argv[1], argv[2])) {
    std::cout << "Perfect match between files " << argv[1] << " and " << argv[2] << "\n";
   }

  return 0;
}