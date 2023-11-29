//
// Created by lhy on 23-10-9.
//
#include"vector.h"
namespace lhy{
std::string str(const vector<char>& obj){
  std::string temp;
  for (auto& each :obj) {
    temp+=each;
  }
  return temp;
}
}