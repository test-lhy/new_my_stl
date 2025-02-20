//
// Created by lhy on 24-8-8.
//
#include <util.h>

#include <source_location>
enum color { a1213123, b, c };
using namespace lhy;
int main() {
  int b = 1;
  const int* c = &b;
  int& a = b;
  std::cout << get_type<std::decay_t<std::reference_wrapper<int>>>() << '\n';
  std::cout << get_type<int, 1>() << '\n';
  std::cout << get_type(a1213123) << '\n';
  std::cout << get_type(color::b) << '\n';
  std::cout << get_type(color::c) << '\n';
}