//
// Created by lhy on 24-4-6.
//
#include "strings.h"
namespace lhy {

template <>
class TypeTraits<string> {
 public:
  static HashFuncType<string> hash_func;
};
HashFuncType<string> TypeTraits<string>::hash_func = [](const string& x, const size_t& mod) -> Index {
  size_t hash = 0;
  // todo:改成可自定义multiplier
  const int multiplier = 233;
  for (auto c : x) {
    hash = (hash * multiplier + c) % mod;
  }
  return hash;
};

std::istream& operator>>(std::istream& istream_, string& obj) {
  obj.clear();
  char temp_char;
  while ((temp_char = static_cast<char>(istream_.get())) == ' ' || temp_char == '\n')
    ;
  istream_.unget();
  while ((temp_char = static_cast<char>(istream_.get())) != EOF && temp_char != ' ' && temp_char != '\n') {
    obj.push_back(temp_char);
  };
  if (temp_char == ' ') {
    istream_.unget();
  }
  return istream_;
}
string& string::operator=(const string& other) = default;
string& string::operator=(string&& other) noexcept = default;
string& string::operator=(const std::initializer_list<char>& other) {
  vector::operator=(other);
  return *this;
}
string& string::operator+=(const char& other) {
  vector::operator+=(other);
  return *this;
}
string& string::operator+=(const string& other) {
  vector::operator+=(other);
  return *this;
}
string& string::operator&=(const string& other) {
  vector::operator&=(other);
  return *this;
}
string& string::operator|=(const string& other) {
  vector::operator|=(other);
  return *this;
}
void getline(std::istream& istream_, string& obj) {
  obj.clear();
  char temp_char;
  while ((temp_char = static_cast<char>(istream_.get())) == '\n')
    ;
  istream_.unget();
  while ((temp_char = static_cast<char>(istream_.get())) != EOF && temp_char != '\n') {
    obj.push_back(temp_char);
  }
}
string operator+(string a, const string& b) { return a += b; }
string operator&(string a, const string& b) { return a &= b; }
string operator|(string a, const string& b) { return a |= b;
}
}
