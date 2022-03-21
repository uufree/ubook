#include "test.h"
#include <iostream>
#include <vector>
#include <list>

using namespace std;

int main() {
  std::vector<int> result{1,2,3,4,5,6,7,8,9,10};
  auto iter = result.begin();

  while (iter != result.end()) {
    iter = result.erase(iter);
  }
  std::cout << result.size() << std::endl;

  for (auto iter=result.begin(); iter!=result.end(); ++iter) {
    std::cout << *iter << std::endl;
  }

  return 0;
}