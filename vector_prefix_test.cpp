//
// Created by lhy on 23-10-10.
//
#include"string.h"
#include<vector>
using namespace lhy;
using std::cin;
using std::cout;
int main(){
  string now;
  string now2;
  cin>>now;
  cin>>now2;
  now[1];
  auto ans= GetKMP(now2,now);
  cout<<ans;
}