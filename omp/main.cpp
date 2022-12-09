#include <iostream>
#include <vector>
#include <stdio.h>
#include <cstdio>
#include "test.h"
using namespace std;
typedef long long ll;
//#define LOCAL//для стандртного ввода закомментировать
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
    init();
    initIO();
    int cmd;
    cout<<"test mode/get ans mode (1/0)  ";cin>>cmd;
    if (cmd){
        test();
    }else get_ans();
}
