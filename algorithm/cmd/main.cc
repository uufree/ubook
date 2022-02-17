#include <iostream>
#include "test.h"

extern int test();

int main() {
  int num = 10;
  int nums[10];
  int *p = &num;
  std::cout << sizeof(p) << std::endl;
  return 0;
}