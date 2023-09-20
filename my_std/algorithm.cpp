//
// Created by lhy31 on 2023/8/26.
//
#include "algorithm.h"
namespace lhy{
std::string ToString(int64_t origin){
  std::string ans;
  while(origin>0){
    ans+=static_cast<char>(origin%10+'0');
    origin/=10;
  }
  reverse(ans.begin(),ans.end());
  return std::move(ans);
}
}