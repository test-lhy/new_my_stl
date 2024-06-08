//
// Created by lhy on 24-6-4.
//
#include "weak_ptr.h"

#include <cassert>
#include <iostream>
// 测试默认构造函数
void test_default_constructor() {
  lhy::weak_ptr<int> wp;
  assert(wp.expired());
  assert(wp.use_count() == 0);
}

// 测试从shared_ptr构造
void test_constructor_from_shared_ptr() {
  lhy::shared_ptr<int> sp(new int(42));
  lhy::weak_ptr<int> wp(sp);
  assert(!wp.expired());
  assert(wp.use_count() == 1);
}

// 测试复制构造函数
void test_copy_constructor() {
  lhy::shared_ptr<int> sp(new int(42));
  lhy::weak_ptr<int> wp1(sp);
  lhy::weak_ptr<int> wp2(wp1);
  assert(!wp2.expired());
  assert(wp2.use_count() == 1);
}

// 测试移动构造函数
void test_move_constructor() {
  lhy::shared_ptr<int> sp(new int(42));
  lhy::weak_ptr<int> wp1(sp);
  lhy::weak_ptr<int> wp2(std::move(wp1));
  assert(!wp2.expired());
  assert(wp2.use_count() == 1);
  assert(wp1.expired());
}

// 测试复制赋值运算符
void test_copy_assignment() {
  lhy::shared_ptr<int> sp(new int(42));
  lhy::weak_ptr<int> wp1(sp);
  lhy::weak_ptr<int> wp2;
  wp2 = wp1;
  assert(!wp2.expired());
  assert(wp2.use_count() == 1);
}

// 测试移动赋值运算符
void test_move_assignment() {
  lhy::shared_ptr<int> sp(new int(42));
  lhy::weak_ptr<int> wp1(sp);
  lhy::weak_ptr<int> wp2;
  wp2 = std::move(wp1);
  assert(!wp2.expired());
  assert(wp2.use_count() == 1);
  assert(wp1.expired());
}

// 测试lock函数
void test_lock() {
  lhy::shared_ptr<int> sp(new int(42));
  lhy::weak_ptr<int> wp(sp);
  lhy::shared_ptr<int> sp2 = wp.lock();
  assert(sp2.use_count() == 2);
  assert(*sp2 == 42);
}

// 测试reset函数
void test_reset() {
  lhy::shared_ptr<int> sp(new int(42));
  lhy::weak_ptr<int> wp(sp);
  wp.reset();
  assert(wp.expired());
  assert(wp.use_count() == 0);
}
#include <iostream>
#include <string>

#include "weak_ptr.h"

// 自定义删除器
struct CustomDeleter {
  void operator()(int* p) {
    std::cout << "Deleting int: " << *p << std::endl;
    delete p;
  }
};

// 基类和派生类
class Base {
 public:
  virtual ~Base() = default;
};

class Derived : public Base {
 public:
  Derived(int value) : value_(value) {}
  ~Derived() { std::cout << "Destroying Derived: " << value_ << std::endl; }
  int value_;
};

// 测试自定义删除器
void test_custom_deleter() {
  lhy::shared_ptr<int> sp(new int(42), CustomDeleter());
  lhy::weak_ptr<int> wp(sp);
  assert(!wp.expired());
  assert(wp.use_count() == 1);
  sp.reset();
  assert(wp.expired());
}

// 测试基类和派生类
void test_base_derived() {
  lhy::shared_ptr<Derived> sp(new Derived(123));
  lhy::weak_ptr<Derived> wp(sp);
  assert(!wp.expired());
  assert(wp.use_count() == 1);
  lhy::shared_ptr<Base> bp(sp);
  lhy::weak_ptr<Base> wp_base(bp);
  assert(!wp_base.expired());
  assert(wp_base.use_count() == 2);
}

// 测试数组
void test_array() {
  lhy::shared_ptr<int[]> sp(new int[5]{1, 2, 3, 4, 5});
  lhy::weak_ptr<int[]> wp(sp);
  assert(!wp.expired());
  assert(wp.use_count() == 1);
  assert(sp[2] == 3);
  lhy::shared_ptr<int[]> sp2 = wp.lock();
  assert(sp2.use_count() == 2);
  assert(sp2[3] == 4);
}
// 节点类,用于测试循环引用
class Node {
 public:
  Node(int value) : value_(value) {}
  ~Node() { std::cout << "Destroying Node: " << value_ << std::endl; }
  int value_;
  lhy::shared_ptr<Node> next_;
  lhy::weak_ptr<Node> prev_;
};

// 测试循环引用
void test_cycle_reference() {
  lhy::shared_ptr<Node> node1(new Node(1));
  lhy::shared_ptr<Node> node2(new Node(2));
  node1->next_ = node2;
  node2->prev_ = node1;

  // 使用weak_ptr打破循环引用

  // 检查节点是否已被正确销毁
}

// 测试嵌套类型
struct Nested {
  int value;
  lhy::shared_ptr<Nested> next;
};

void test_nested_types() {
  lhy::shared_ptr<Nested> sp(new Nested{42, nullptr});
  lhy::weak_ptr<Nested> wp(sp);
  assert(!wp.expired());
  assert(wp.use_count() == 1);
  assert(wp.lock()->value == 42);
}

int main() {
  test_default_constructor();
  test_constructor_from_shared_ptr();
  test_copy_constructor();
  test_move_constructor();
  test_copy_assignment();
  test_move_assignment();
  test_lock();
  test_reset();
  test_custom_deleter();
  test_base_derived();
  test_array();
  test_cycle_reference();
  test_nested_types();
  std::cout << "All tests passed!" << std::endl;
  return 0;
}