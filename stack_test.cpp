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
  lhy::stack<int> s2;

  s2.push(5);
  s2.push(6);
  s2.push(7);
  lhy::stack<int> s1(move(s2));
  cout<<s1.empty()<<s1.top()<<endl;
  cout << "Stack: " << s.show() << endl;

  cout << "Top: " << s.top() << endl;

  s.pop();

  cout << "Stack after pop: " << s.show() << endl;

  cout << "Empty? " << s.empty() << endl;

  s.clear();
  //s1=s;
  cout << "Empty after clear? " << s.empty() << endl;
  
  return 0;
}