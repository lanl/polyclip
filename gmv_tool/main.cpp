
#include "gmv_writer.h"
#include <iostream>
#include <fstream>
int main(int argc, char* argv[]){

  std::ofstream clear_file("test.gmv", std::ios::trunc);
  std::ofstream test_file("test.gmv", std::ios::app);

  test_file << "gmvinput ascii\n";
  test_file << "nodev 11\n";
  test_file << std::scientific << std::setprecision(17);
  test_file << 0.0  << " " << 0.0 << " " << 0.0 << "\n";
  test_file << 0.5  << " " << 0.0 << " " << 0.0 << "\n";
  test_file << 0.5  << " " << 0.25 << " " << 0.0 << "\n";
  test_file << 0.25  << " " << 0.375 << " " << 0.0 << "\n";
  test_file << 0.0  << " " << 0.25 << " " << 0.0 << "\n";
  test_file << 0.875  << " " << 0.25 << " " << 0.0 << "\n";
  test_file << 1.0  << " " << 0.375 << " " << 0.0 << "\n";
  test_file << 0.875  << " " << 0.625 << " " << 0.0 << "\n";
  test_file << 0.5  << " " << 0.625 << " " << 0.0 << "\n";
  test_file << 0.5  << " " << 0.875 << " " << 0.0 << "\n";
  test_file << 0.875  << " " << 0.875 << " " << 0.0 << "\n";
  test_file << "cells " << 4 << "\n";
  test_file << "general 1 5 1 2 3 4 5\n";
  test_file << "general 1 3 2 6 3\n";
  test_file << "general 1 6 3 6 7 8 9 4\n";
  test_file << "general 1 4 9 8 11 10\n";

  // test_file << "general 2";
  // test_file << "general 3";
  // test_file << "general 4";
  // test_file << "general 5";
  test_file << "endgmv\n";

  test_file.close();
  std::cout << "I like eggs. . " << std::endl;


  return 0;
}
