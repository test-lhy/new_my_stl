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
    std::cout << "contain 56(1):" << a.Exist(56) << std::endl;
    std::cout << "contain 50(0):" << a.Exist(50) << std::endl;
    for (auto [index, content] : a) {
      std::cout << index << " " << content << " | ";
    }
    std::cout << std::endl;
    a.erase(56);
    std::cout << "contain 56(0):" << a.Exist(56) << std::endl;
    std::cout << "contain 100(1):" << a.Exist(100) << std::endl;
    a.erase(4);
    for (auto [index, content] : a) {
      std::cout << index << " " << content << " | ";
    }
    std::cout << std::endl;
    a[56] = 100;
    for (auto [index, content] : a) {
      std::cout << index << " " << content << " | ";
    }
    std::cout << std::endl;
    a[56] = 40;
    for (auto [index, content] : a) {
      std::cout << index << " " << content << " | ";
    }
    std::cout << std::endl;
    for (auto iter = a.rbegin(); iter != a.rend(); iter++) {
      std::cout << iter->first << " " << iter->second << " | " << std::flush;
    }
    std::cout << std::endl;
  }
}
