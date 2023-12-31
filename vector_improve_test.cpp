//
// Created by lhy on 23-10-9.
//
#include <sstream>

#include "vector.h"

using namespace lhy;
using std::cin;
using std::cout;
using std::endl;
using std::stringstream;
int main() {
  vector<int> v1{1, 5, 3};
  vector<int> v2{4, 5, 6};

  // Test += operator
  v1 += v2;
  cout << "v1 after += v2:" << endl;
  // Output: 1 2 3 4 5 6
  cout << v1 << endl;

  // Test &= operator
  v1 = {1, 5, 3};
  v2 = {4, 5, 6};
  v1 &= v2;
  cout << "v1 after &= v2:" << endl;
  // Output:
  cout << v1 << endl;

  // Test | operator
  v1 = {1, 5, 3};
  v2 = {4, 5, 6};
  v1 |= v2;
  cout << "v1 after |= v2:" << endl;
  // Output: 1 2 3 4 5 6
  cout << v1 << endl;

  // Test input stream operator
  vector<int> v3;
  stringstream ss("7 8 9");
  ss >> v3;
  cout << "v3 after reading from stringstream:" << endl;
  // Output: 7 8 9
  cout << v3 << endl;

  // Test input stream operator for vector<char>
  vector<char> v4;
  stringstream ss2("abc");
  ss2 >> v4;
  cout << "v4 after reading from stringstream:" << endl;
  // Output: abc
  cout << v4 << endl;

  return 0;
}