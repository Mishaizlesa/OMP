
#ifndef kmp_h
#define kmp_h
#include <omp.h>
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
std::ofstream fout("/Users/mihailkozlov/Desktop/hash3_2.txt");
typedef long long ll;
class Find_Substr{
private:
    int ord[256];
    std::string data;
    ll size;
    static char symb[4];
    long long* powmod;
    const int p=31;
    const int mod=1e9+7;
    void prefix_func(const std::string &s, int* p) {
        int k=0;
        p[0]=0;
        for (int i=1; i<s.length();++i) {
            while (k>0 && s[k]!=s[i]) {
                k=p[k-1];
            }
            if (s[k]==s[i]){
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
    static std::uniform_int_distribution<int> d;
    static std::default_random_engine rnd;
    
    
    Find_Substr(int len){
        ord['A']=0;ord['C']=1;ord['G']=2;ord['T']=3;
        powmod=new long long[len];
        size=len;
        data=str_generator(len);
        powmod[0]=1;
        for(int i=1;i<size;++i) powmod[i]=(p*powmod[i-1])%mod;
    }
    
    
    Find_Substr(const std::string& str){
        ord['A']=0;ord['C']=1;ord['G']=2;ord['T']=3;
        data=str;
        size=data.size();
        powmod=new long long[size];
        for(int i=1;i<size;++i) powmod[i]=(p*powmod[i-1])%mod;
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
        p_c=new int*[24];
        for(int i=0;i<24;++i){
            p_c[i]=new int[size+len+1];
        }
#pragma omp parallel for shared(freq,len) num_threads(24)
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
    
    
    
    
    std::vector<int> rabin_karp(const int& len, const int& th){
        std::vector<int>freq(size);
        std::vector<ll>dp_h(size+1);
        dp_h[0]=0;
        for(int i=0;i<size;++i){
            if (data[i]=='A') dp_h[i+1]=(dp_h[i]+powmod[i])%mod;
            else if (data[i]=='C') dp_h[i+1]=(dp_h[i]+2LL*powmod[i])%mod;
            else if (data[i]=='G') dp_h[i+1]=(dp_h[i]+3LL*powmod[i])%mod;
            else  dp_h[i+1]=(dp_h[i]+4LL*powmod[i])%mod;
        }
#pragma omp parallel for shared(freq,len,dp_h) num_threads(th)
        {
            for(int i=0;i<=size-len;++i){
                ll hash=0;
                for(int j=0;j<len;++j){
                    if (data[i+j]=='A') hash=(hash+powmod[j])%mod;
                    else if (data[i+j]=='C') hash=(hash+2LL*powmod[j])%mod;
                    else if (data[i+j]=='G') hash=(hash+3LL*powmod[j])%mod;
                    else  hash=(hash+4LL*powmod[j])%mod;
                }
                for(int j=0;j<=size-len;++j){
                    ll val_h=(dp_h[j+len]+mod-dp_h[j])%mod;
                    if (val_h==(hash*powmod[j])%mod && data.substr(i,len)==data.substr(j,len)) freq[i]++;
                }
            }
        }
        return freq;
    }
    
    
    
    std::vector<int> hash3(const int& len, const int& th){
        std::vector<int>freq(size);
#pragma omp parallel for shared(freq,len) num_threads(th)
        {
            for(int i=0;i<=size-len;++i){
                int sh1;
                std::vector<int>shift(64,len-2);
                ll hash=0;
                for(int j=2;j<=len-1;++j){
                    int ind=ord[data[i+j-2]]*16+ord[data[i+j-1]]*4+ord[data[i+j]];
                    if (j==len-1) sh1=shift[ind];
                    shift[ind]=len-1-j;
                }
                
                if (!sh1) sh1=1;
                int j=len-1;
                
                for(;;){
                    int sh=1;
                    while (sh && j<size) {
                        int ind=ord[data[j-2]]*16+ord[data[j-1]]*4+ord[data[j]];
                        sh=shift[ind];
                        j+=sh;
                    }
                    if (j<size){
                        int is_eq=1;
                        for(int k=0;k<len;++k){
                            if (data[i+k]!=data[j-len+1+k]){
                                is_eq=0;
                                break;
                            }
                        }
                        freq[i]+=is_eq;
                        j+=sh1;
                    }else{
                        break;
                    }
                }
            }
        }
        return freq;
    }
    
    void boyer_moore_bad_char(std::string& str, std::vector<ll>&bad_char ,int size, int th){
        {
            for(int i=0;i<size-2;++i)
            {
                bad_char[str[i]]=size-1-i;
            }
        }
    }
    
    void boyer_moore_good_suff(std::string& pat, std::vector<ll>& shift, std::vector<ll>& bpos, int m){
        int i=m;
        int j=m+1;
        bpos[i]=j;
        while (i>0) {
            while (j<=m && pat[i-1]!=pat[j-1]) {
                if (shift[j]==0) shift[j]=j-i;
                j=bpos[j];
            }
            i--;j--;
            bpos[i]=j;
        }
    }
    
    void boyer_moore_prep(std::string& pat, std::vector<ll>& shift, std::vector<ll>& bpos, int m, int th){
        int j;
        j=bpos[0];
        for(int i=0;i<m;++i){
            if (shift[i]==0){
                shift[i]=j;
            }
            if (i==j) j=bpos[j];
        }
    }
    
    std::vector<int> boyer_moore(int len, int th){
        std::vector<int>freq(size);
#pragma omp parallel for shared(freq,len) num_threads(th)
        {
            for(int i=0;i<=size-len;++i){
                if (i==2){
                    int cff=0;
                }
                int ptr=0;
                int u=0;
                int sh=len;
                std::vector<ll>gs(len+1,0);
                
                std::vector<ll>bpos(len+1);
                
                std::vector<ll>bad_char(256,len);
                
                std::string pat=data.substr(i,len);
                
                boyer_moore_bad_char(pat,bad_char,len,th);
                
                boyer_moore_good_suff(pat,gs,bpos,len);
                
                boyer_moore_prep(pat,gs,bpos,len,th);
                
                while (ptr<=size-len) {
                    int j=len-1;
                    while (j>=0 && pat[j]==data[ptr+j]) {
                        j--;
                        if (u!=0 && j==len-1-sh){
                            j-=u;
                        }
                    }
                    if (j<0){
                        freq[i]++;
                        sh=gs[0];
                        u=len-sh;
                    }else{
                        ll v=len-1-j;
                        ll turbo=u-v;
                        ll bcsh=bad_char[data[ptr+j]]-len+j+1;
                        sh=std::max({turbo, bcsh, gs[j+1]});
                        if (sh==gs[j+1]){
                            u=(len-sh>v?v:len-sh);
                        }else{
                            if (turbo<bcsh){
                                sh=(sh>u+1?u+1:sh);
                            }
                            u=0;
                        }
                    }
                    ptr+=sh;
                }
            }
        }
        return freq;
    }
    
    
    
    
    void get_ans(const int& len){
        int mmax=0;
        auto res=hash3(len,24);
        std::vector<std::unordered_set<std::string>>num(size+1);
        std::string tmp;
        for(int i=0;i<res.size();++i){
            mmax=std::max(mmax,res[i]);
            for(int j=0;j<len;++j) tmp+=data[i+j];
            num[res[i]].insert(tmp);
            tmp.clear();
        }
        std::cout<<"text: "<<data<<"\n";
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
    
    
    
    void parallel_test(){
        
        int len=(size/10>256?256:size/10);
        double time_s,dif;
        std::vector<double>res;
        time_s=omp_get_wtime();
        rabin_karp(len,24);
        dif=omp_get_wtime()-time_s;
        res.push_back(dif);
        time_s=omp_get_wtime();
        kmp_parallel(len);
        dif=omp_get_wtime()-time_s;
        res.push_back(dif);
        if (len<3) len=3;
        time_s=omp_get_wtime();
        hash3(len,24);
        dif=omp_get_wtime()-time_s;
        res.push_back(dif);
        time_s=omp_get_wtime();
        boyer_moore(len,24);
        dif=omp_get_wtime()-time_s;
        res.push_back(dif);
        fout<<size<<" ";
        for(auto el: res) fout<<el<<" ";
        fout<<"\n";
        //kmp_parallel(len);
    }
    
    
    
    void test_seq(){
        int len=(size/10>256?256:size/10);
        double time_s,dif;
        std::vector<double>res;
        time_s=omp_get_wtime();
        rabin_karp(len,1);
        dif=omp_get_wtime()-time_s;
        res.push_back(dif);
        time_s=omp_get_wtime();
        kmp_default(len);
        dif=omp_get_wtime()-time_s;
        res.push_back(dif);
        if (len<3) len=3;
        time_s=omp_get_wtime();
        hash3(len,1);
        dif=omp_get_wtime()-time_s;
        res.push_back(dif);
        time_s=omp_get_wtime();
        boyer_moore(len,1);
        dif=omp_get_wtime()-time_s;
        res.push_back(dif);
        fout<<size<<" ";
        for(auto el: res) fout<<el<<" ";
        fout<<"\n";
    }
    
    
    
    
    void test_pat_size(){
        for(int len=6;len<=size;len*=5){
            double time_s,dif;
            std::vector<double>res;
            time_s=omp_get_wtime();
            rabin_karp(len,24);
            dif=omp_get_wtime()-time_s;
            res.push_back(dif);
            time_s=omp_get_wtime();
            kmp_parallel(len);
            dif=omp_get_wtime()-time_s;
            res.push_back(dif);
            if (len<3) len=3;
            time_s=omp_get_wtime();
            hash3(len,24);
            dif=omp_get_wtime()-time_s;
            res.push_back(dif);
            time_s=omp_get_wtime();
            boyer_moore(len,24);
            dif=omp_get_wtime()-time_s;
            res.push_back(dif);
            fout<<len<<" ";
            for(auto el: res) fout<<el<<" ";
            fout<<"\n";
        }
    }
    void hash3_tst(){
        int len=(size/10>256?256:size/10);
        if (len<3) len=3;
        double time_s,dif;
        std::vector<double>res;
        time_s=omp_get_wtime();
        hash3(len,24);
        dif=omp_get_wtime()-time_s;
        res.push_back(dif);
        time_s=omp_get_wtime();
        //hash3(len,1);
        dif=omp_get_wtime()-time_s;
        res.push_back(dif);
        fout<<size<<" ";
        for(auto el: res) fout<<el<<" ";
        fout<<"\n";
        //std::cout<<size<<"\n";
    }
};
#endif /* kmp_h */
