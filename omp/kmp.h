
#ifndef kmp_h
#define kmp_h
#include "omp.h"
#include <iostream>
#include <vector>
#include <stdio.h>
#include <cstdio>
#include <unordered_map>
#include <unordered_set>
using namespace std;
typedef long long ll;
vector<int>prefix_func(const string &s) {
    vector<int>p(s.length());
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
    return p;
}
vector<int>kmp_deafult(const string& txt, int len){
    vector<int>freq(txt.size());
    for(ll i=0;i<=txt.size()-len;++i){
        string tmp=txt.substr(i,len);
        vector<int>p_c=prefix_func(tmp+string(1,'#')+txt);
        for(int j=0;j<txt.size();++j){
            if (p_c[len+j+1]==len){
                freq[i]++;
            }
        }
    }
    return freq;
}
vector<int> kmp_parallel(const string& txt, const int& len){
    vector<int>freq(txt.size());

#pragma omp parallel for shared(freq,txt,len)
    {
        for(ll i=0;i<=txt.size()-len;++i){
            string tmp=txt.substr(i,len);
            vector<int>p_c=prefix_func(tmp+string(1,'#')+txt);
            for(int j=0;j<txt.size();++j){
                if (p_c[len+j+1]==len){
                    freq[i]++;
                }
            }
        }
    }
    return freq;
}
void get_data(string& str, int& len){
    cin>>str;cin>>len;
}
void get_ans(){
    string txt;int len;
    get_data(txt,len);
    int mmax=0;
    auto res=kmp_parallel(txt, len);
    vector<unordered_set<string>>num(txt.size()+1);
    string tmp;
    for(int i=0;i<res.size();++i){
        mmax=max(mmax,res[i]);
        for(int j=0;j<len;++j) tmp+=txt[i+j];
        num[res[i]].insert(tmp);
        tmp.clear();
    }
    cout<<"highest frerquency = "<<mmax<<"\n";
    cout<<"most common strings:\n";
    for(auto& el: num[mmax]){
        cout<<el<<"\n";
    }
}
#endif /* kmp_h */
