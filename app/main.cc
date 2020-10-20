// File: main.cc

#include <iostream>

#include <toyMax.h>

using namespace ilang;

int main() {
  auto m = toyMax::GetToymaxIla("toyMax");
  std::cout << "Successfully construct " << m << "\n";
  return 0;
}
