#ifndef test_h
#define test_h
#include "omp.h"
#include "kmp.h"
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
const char symb[]={'A','C','G','T'};
//freopen("local.out", "w", stdout);
ofstream fout("/Users/mihailkozlov/Desktop/omp/omp/omp/test_result.txt");
string str_generator(int len){
    string res(len,'0');
    for(int i=0; i<len;++i){
        res[i]=symb[rand()%4];
    }
    return res;
}
void test(){
    srand(time(NULL));
    for(int i=100;i<=1e5;i*=10){
        auto txt=str_generator(i);
        int len=i/10;
        double st_par=clock();
        kmp_parallel(txt, len);
        double fin_par=clock()-st_par;
        double st_def=clock();
        kmp_parallel(txt, len);
        double fin_def=clock()-st_def;
        fout<<i<<" "<<fin_par<<" "<<fin_def<<"\n";
        
    }
}
#endif /* test_h */
