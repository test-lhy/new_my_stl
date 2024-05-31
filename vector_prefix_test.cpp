//
// Created by lhy on 23-10-10.
//
#include "strings.h"
using namespace lhy;
using std::cin;
using std::cout;
int main() {
  string now;
  cin >> now;
  int n;
  cin >> n;
  auto DFA = GetKmpDFA(now);
  for (int i = 0; i < n; ++i) {
    string now1;
    cin >> now1;
    auto ans = GetKMP(DFA, now.size(), now1);
    cout << ans;
    cout << std::endl;
  }
}