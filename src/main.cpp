#include <cstdlib>

#include "AssemblerEngine.hpp"

int main(int argc, char** argv) {
  Hasm::AssemblerEngine assembler;

  return assembler.run(argc, argv) ? EXIT_SUCCESS : EXIT_FAILURE;
}
