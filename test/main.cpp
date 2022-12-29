#include "kmp.h"
#include <time.h>
#include <stdio.h>
#include <iostream>
char Find_Substr::symb[4]={'A','C','G','T'};
std::uniform_int_distribution<int> Find_Substr::d(1,4);
std::default_random_engine Find_Substr::rnd(std::chrono::system_clock::now().time_since_epoch().count());
int main(){
    fout<<"num of elems parallel_kmp deafault_kmp pseudo_parallel_kmp\n";
    for(int i=10;i<=1e5;i+=i/10){
        Find_Substr a(i);
        a.test_kmp();
    }
}

