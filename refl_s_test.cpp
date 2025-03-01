//
// Created by 连昊宇 on 25-2-19.
//
#include <macro.h>
#include <refl_s.h>

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
using namespace lhy;

class base {
 public:
  int a1;
  int a2;
  // int f1();

 protected:
  int a3;
  // int f2();

 private:
  double a4{0.1};
  // int f3();
};
REFL(base, a1, a2, a3, a4)

class node2 {
 public:
  int a;
  int b, c;
  float d;
  char e;
  double f;
  bool g;

  node2() {};
  node2(int a, int b, int c, float d, char e, double f, bool g) : a(a), b(b), c(c), d(d), e(e), f(f), g(g) {}
  ~node2() = default;
  node2(const node2& other) = default;
  node2(node2&& other) = default;
  node2& operator=(const node2& other) = default;
  node2& operator=(node2&& other) = default;
  void h();
  inline static int m{114514};
  static int m1() { return 1; };
  std::string n;
  base n1;
  int n2[10];

 protected:
  inline static int m2{114514};
  void i();
  int j;

 private:
  inline static int m3{114514};
  int k;
  void l();
};
REFL(node2, a, b, c, d, e, f, g, n, j, k, m, m2, m3, n1)

void test1() {
  auto t = refl_s<base>::members;
  base a;
  assert(a.*(std::get<3>(t)) == 0.1);
}
void test2() {
  base a;
  auto& c = GetMember<double>(a, "a4");
  std::cout << c << std::endl;
  c = 5;
  assert(c == 5);
  c = 10;
  assert(c == 10);

  const base a1{};
  auto& c1 = GetMember<double>(a1, "a4");
  static_assert(std::is_same_v<decltype(c1), const double&>);
}
void test3() {
  std::cout << GetMemberAccessSpecifierInString<base>("a1") << std::endl;
  std::cout << GetMemberAccessSpecifierInString<base>("a2") << std::endl;
  std::cout << GetMemberAccessSpecifierInString<base>("a3") << std::endl;
  std::cout << GetMemberAccessSpecifierInString<base>("a4") << std::endl;
  assert(GetMemberAccessSpecifierInString<base>("a1") == "public");
  assert(GetMemberAccessSpecifierInString<base>("a2") == "public");
  assert(GetMemberAccessSpecifierInString<base>("a3") == "protected");
  assert(GetMemberAccessSpecifierInString<base>("a4") == "private");
}
void test4() {
  node2 a;
  auto& c = GetMember<int>(a, "m3");
  auto& c1 = GetStaticMember<int, node2>("m3");
  std::cout << c << std::endl;
  std::cout << c1 << std::endl;
  assert(c == c1);
  c = 5;
  assert(c == c1);
  assert(c == 5);
  c = 10;
  assert(c == c1);
  assert(c == 10);
  const node2 a1{};
  auto& c2 = GetMember<int>(a1, "m3");
  static_assert(std::is_same_v<decltype(c2), const int&>);
}
void test5() {
  std::cout << GetMemberAccessSpecifierInString<node2>("m") << std::endl;
  std::cout << GetMemberAccessSpecifierInString<node2>("m2") << std::endl;
  std::cout << GetMemberAccessSpecifierInString<node2>("m3") << std::endl;
  assert(GetMemberAccessSpecifierInString<node2>("m") == "public");
  assert(GetMemberAccessSpecifierInString<node2>("m2") == "protected");
  assert(GetMemberAccessSpecifierInString<node2>("m3") == "private");
}
void test6() {
  node2 a;
  GetMember<int>(a, "a") = 1;
  GetMember<int>(a, "b") = 2;
  GetMember<int>(a, "c") = 3;
  GetMember<float>(a, "d") = 4.4;
  GetMember<char>(a, "e") = 'a';
  GetMember<double>(a, "f") = 5.5;
  GetMember<bool>(a, "g") = true;
  GetMember<int>(a, "m") = 6;
  GetMember<int>(a, "m2") = 7;
  GetMember<int>(a, "j") = 8;
  GetMember<int>(a, "k") = 9;
  GetMember<std::string>(a, "n") = "10";
  auto& n1 = GetMember<base>(a, "n1");
  GetMember<int>(n1, "a1") = 11;
  GetMember<int>(n1, "a2") = 12;
  GetMember<int>(n1, "a3") = 13;
  GetMember<double>(n1, "a4") = 14;
  std::cout << serialize(a) << std::endl;
  node2 a2;
  deserialize(a2, serialize(a));
  std::cout << serialize(a2) << std::endl;
}
int main() {
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  std::cout << "all test passed";
}