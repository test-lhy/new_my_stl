//
// Created by lhy on 24-4-22.
//
#include "index_container.h"
using namespace lhy;
int main() {
  auto b = {VectorIndexContainerMode, MapIndexContainerMode};
  for (auto &eachmode : b) {
    std::cout << eachmode << std::endl;
    IndexContainer<int> a(eachmode);
    a[1] = 1;
    a[4] = 2;
    a[100] = 3;
    a[56] = 4;
    for (auto each : a) {
      std::cout << each << " ";
    }
    std::cout << std::endl;
    a.erase(56);
    a.erase(4);
    for (auto each : a) {
      std::cout << each << " ";
    }
    std::cout << std::endl;
    a[56] = 100;
    for (auto each : a) {
      std::cout << each << " ";
    }
    std::cout << std::endl;
    a[56] = 40;
    for (auto each : a) {
      std::cout << each << " ";
    }
    std::cout << std::endl;
  }
}
