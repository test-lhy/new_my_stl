//
// Created by 连昊宇 on 25-2-19.
//
#include <macro.h>
#include <refl_s.h>

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

// class node2 {
//   int a;
//   int b, c;
//   float d;
//   char e;
//   double f;
//   bool g;
//
//  public:
//   node2() {};
//   node2(int a, int b, int c, float d, char e, double f, bool g) : a(a), b(b), c(c), d(d), e(e), f(f), g(g) {}
//   ~node2() = default;
//   node2(const node2& other) = default;
//   node2(node2&& other) = default;
//   node2& operator=(const node2& other) = default;
//   node2& operator=(node2&& other) = default;
//   void h();
//   static int m;
//   static int m1() {};
//
//  protected:
//   void i();
//   int j;
//
//  private:
//   int k;
//   void l();
// };
// struct node : base {
//   using type_now = int;
//   int a;
//   int b, c;
//   float d;
//   char e;
//   double f;
//   bool g;
//   node() = default;
//   node(int a, int b, int c, float d, char e, double f, bool g) : a(a), b(b), c(c), d(d), e(e), f(f), g(g) {}
//   ~node() = default;
//   node(const node& other) = default;
//   node(node&& other) = default;
//   node& operator=(const node& other) = default;
//   node& operator=(node&& other) = default;
//   void h();
//   node2 n;
//   static int m;
//   static int m1() {};
//
//  protected:
//   void i();
//   int j;
//
//  private:
//   int k;
//   void l();
// };

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
int main() {
  test1();
  test2();
  test3();
  std::cout << "all test passed";
}