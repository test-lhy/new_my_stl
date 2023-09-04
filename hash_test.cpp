//
// Created by lhy31 on 2023/9/4.
//
#include "hash.h"
#include <string>
#include<iostream>

int main() {

  lhy::hash<std::string, int> hash_table;

  hash_table.insert("apple", 1);
  hash_table.insert("banana", 2);
  hash_table.insert("orange", 3);

  std::cout<<hash_table["apple"] ;
  std::cout<<hash_table["banana"];
  std::cout<<hash_table["orange"];

  hash_table["apple"] = 4;
  std::cout<<hash_table["apple"];

  std::cout << "All tests passed!" << std::endl;

  return 0;
}