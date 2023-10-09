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
template<>
void getline<char>(std::istream& istream_,vector<char>& obj){
  obj.clear();
  char temp_char;
  while((temp_char=static_cast<char>(istream_.get()))=='\n');
  istream_.unget();
  while((temp_char=static_cast<char>(istream_.get()))!=EOF&&temp_char!='\n'){
    obj.push_back(temp_char);
  }
}
template<>
std::istream& operator>><char>(std::istream& istream_,vector<char>& obj){
  obj.clear();
  char temp_char;
  while((temp_char=static_cast<char>(istream_.get()))==' '||temp_char=='\n');
  istream_.unget();
  while((temp_char=static_cast<char>(istream_.get()))!=EOF&&temp_char!=' '&&temp_char!='\n'){
    obj.push_back(temp_char);
  };
  if (temp_char==' '){
    istream_.unget();
  }
  return istream_;
}
template<>
std::ostream& operator<<<char>(std::ostream& ostream_,vector<char>& obj){
  for(auto& each:obj){
    ostream_<<each;
  }
  return ostream_;
}
}