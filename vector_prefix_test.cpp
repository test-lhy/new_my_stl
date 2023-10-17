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
  cin>>now;
  int n;
  cin>>n;
  for (int i = 0; i < n; ++i) {
    string now1;
    cin>>now1;
    auto ans=GetKMP(now1,now);
    cout<<ans;
    cout<<std::endl;
  }
}