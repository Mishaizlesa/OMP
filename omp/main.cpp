#include <iostream>
#include <vector>
#include <stdio.h>
#include <cstdio>
#include "kmp.h"
using namespace std;
typedef long long ll;
//#define LOCAL//для стандртного ввода закомментировать
char Find_Substr::symb[4]={'A','C','G','T'};
std::uniform_int_distribution<int> Find_Substr::d(1,4);
std::default_random_engine Find_Substr::rnd(std::chrono::system_clock::now().time_since_epoch().count());
void initIO()
{
#ifdef LOCAL
    freopen("/Users/mihailkozlov/Desktop/omp/omp/omp/local.in", "r", stdin);
    //freopen("local.out", "w", stdout);
#else
    const string NAME = "input";
    //freopen(string("input.txt").c_str(), "r", stdin);
    //freopen(string("output.txt").c_str(), "w", stdout);
#endif
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);
}
void init(){
    cout<<"threads num = "<<omp_get_max_threads()<<"\n";
}
int main(){
    Find_Substr a(1000000);
    a.test_kmp();
}
