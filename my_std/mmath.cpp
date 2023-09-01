//
// Created by lhy31 on 2023/8/26.
//
#include "mmath.h"
namespace lhy{
int64_t Pow(int64_t a,int64_t b){
  int64_t ans=1;
  while(b>0){
    ans*=(b&1)*a;
    a*=a;
    b>>=1;
  }
  return ans;
}
int64_t Pow(int64_t a,int64_t b,int64_t mod){
  int64_t ans=1;
  a%=mod;
  while(b>0){
    ans=ans*(b&1)*a%mod;
    a=a*a%mod;
    b>>=1;
  }
  return ans;
}
int64_t PowModPrime(int64_t a,int64_t b,int64_t mod){
  int64_t ans=1;
  a%=mod;
  b%=(mod-1);
  while(b>0){
    ans=ans*(b&1)*a%mod;
    a=a*a%mod;
    b>>=1;
  }
  return ans;
}
}