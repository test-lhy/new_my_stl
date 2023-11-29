//
// Created by lhy31 on 2023/9/1.
//
#include "stack.h"

#include <iostream>

using namespace std;

int main() {

  lhy::stack<int> s;

  s.push(1);
  s.push(2);
  s.push(3);

  cout << "Stack: " << s.show() << endl;

  cout << "Top: " << s.top() << endl;

  s.pop();

  cout << "Stack after pop: " << s.show() << endl;

  cout << "Empty? " << s.empty() << endl;

  s.clear();

  cout << "Empty after clear? " << s.empty() << endl;
  return 0;
}