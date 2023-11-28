//
// Created by lhy31 on 2023/9/4.
//

#include <iostream>
#include <string>
#include "unordered_map.h"
int main() {

  lhy::unordered_map<std::string,std::string> hash_table;

  std::cout<<hash_table.equal("orange","orange");
  std::cout<<hash_table.equal("orange","banana");
  std::cout<<hash_table.equal("banana","orange");
  std::cout<<hash_table.equal("banana","banana");
  std::cout<<hash_table.equal("apple","orange");
  std::cout<<hash_table.equal("apple","banana");
  std::cout << "All tests passed!" << std::endl;

  return 0;
}