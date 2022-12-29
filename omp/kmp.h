
#ifndef kmp_h
#define kmp_h
#include "omp.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <cstdio>
#include <exception>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include <random>
std::ofstream fout("/Users/mihailkozlov/Desktop/omp/omp/omp/test_result.txt");
typedef long long ll;
class Find_Substr{
private:
    std::string data;
    ll size;
    void prefix_func(const std::string &s, int* p) {
        int k=0;
        p[0]=0;
        for (int i=1; i<s.length();++i) {
            while (k>0 && s[k]!=s[i]) {
                k=p[k-1];
            }
            if (s[k]==s[i]) {
                k++;
            }
            p[i]=k;
        }
    }
    std::string str_generator(int len){
        std::string res(len,'0');
        for(int i=0; i<len;++i){
            res[i]=symb[d(rnd)];
        }
        return res;
    }
public:
    static char symb[4];
    static std::uniform_int_distribution<int> d;
    static std::default_random_engine rnd;
    Find_Substr(int len){
        size=len;
        data=str_generator(len);
    }
    Find_Substr(const std::string& str){
        data=str;
        size=data.size();
    }
    std::vector<int>kmp_default(const int& len){
        std::vector<int>freq(size);
        int* p_c=new int[len+size+1];
        for(ll i=0;i<=size-len;++i){
            std::string tmp=data.substr(i,len);
            prefix_func(tmp+std::string(1,'#')+data,p_c);
            for(int j=0;j<size;++j){
                if (p_c[len+j+1]==len){
                    freq[i]++;
                }
            }
        }
        delete[] p_c;
        return freq;
    }
    std::vector<int> kmp_parallel(const int& len){
        std::vector<int>freq(size);
        int** p_c;
        p_c=new int*[8];
        for(int i=0;i<8;++i){
            p_c[i]=new int[size+len+1];
        }
    #pragma omp parallel for shared(freq,len) num_threads(8)
        {
            for(ll i=0;i<=size-len;++i){
                std::string tmp=data.substr(i,len);
                int num=omp_get_thread_num();
                prefix_func(tmp+std::string(1,'#')+data,p_c[num]);
                for(int j=0;j<size;++j){
                    if (p_c[num][len+j+1]==len){
                        freq[i]++;
                    }
                }
            }
        }
        for(int i=0;i<8;++i){
            delete[] p_c[i];
        }
        delete[] p_c;
        return freq;
    }
    std::vector<int> kmp_pseudo_parallel(const int& len){
        std::vector<int>freq(size);
        int* p_c=new int[len+size+1];
    #pragma omp parallel for shared(freq,len,data) num_threads(1)
        {
            for(ll i=0;i<=size-len;++i){
                std::string tmp=data.substr(i,len);
                prefix_func(tmp+std::string(1,'#')+data,p_c);
                for(int j=0;j<size;++j){
                    if (p_c[len+j+1]==len){
                        freq[i]++;
                    }
                }
            }
        }
        delete[] p_c;
        return freq;
    }
    void get_ans(const int& len){
        int mmax=0;
        auto res=kmp_parallel(len);
        std::vector<std::unordered_set<std::string>>num(size+1);
        std::string tmp;
        for(int i=0;i<res.size();++i){
            mmax=std::max(mmax,res[i]);
            for(int j=0;j<len;++j) tmp+=data[i+j];
            num[res[i]].insert(tmp);
            tmp.clear();
        }
        std::cout<<"highest frerquency = "<<mmax<<"\n";
        std::cout<<"most common strings:\n";
        for(auto& el: num[mmax]){
            std::cout<<el<<"\n";
        }
    }
    void test_kmp(){
        double time_s,dif;
        std::vector<double>res;
        int len=int(size/10);
        time_s=omp_get_wtime();
        kmp_parallel(len);
        dif=omp_get_wtime()-time_s;
        res.push_back(dif);
        time_s=omp_get_wtime();
        kmp_default(len);
        dif=omp_get_wtime()-time_s;
        res.push_back(dif);
        time_s=omp_get_wtime();
        kmp_pseudo_parallel(len);
        dif=omp_get_wtime()-time_s;
        res.push_back(dif);
        fout<<size<<" "<<res[0]<<" "<<res[1]<<" "<<res[2]<<"\n";
    }
};
#endif /* kmp_h */
