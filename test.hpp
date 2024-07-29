#pragma once
#include <iostream>
#include <random>
#include"scan.hpp"

#define TIME(f) { \
    auto start = std::chrono::high_resolution_clock::now(); \
    f; \
    auto end = std::chrono::high_resolution_clock::now(); \
    std::chrono::duration<double> elapsed = end - start; \
    std::cout << "Elapsed time: " << elapsed.count() << " s\n"; \
}

void show(int* a, size_t n){
    for(int i=0;i<n;i++){
        std::cout<<a[i]<<" ";
    }
    std::cout<<std::endl;
}

template<typename T,class Dis>
T* random_data_gen(int n,Dis dis){
    int *a=new T[n];
    std::random_device r;
    std::default_random_engine e(r());
    for(int i=0;i<n;i++)
        a[i] = dis(e);
    return a;
}

template<typename T>
bool check(T*a,T*b,int n){
    for(int i=0;i<n;i++){
        if(a[i]!=b[i]){
            std::cout<<"Error at index "<<i<<std::endl;
            std::cout<<"Expected "<<b[i]<<" but got "<<a[i]<<std::endl;
            return false;
        }
    }
    return true;
}

void test_scan(){
    std::uniform_int_distribution<int>dis(0,4);
    std::bernoulli_distribution bd(0.8);
    int n=1000000;
    int init =0;
    int *a= random_data_gen<int>(n,dis);
    int *b = new int[n];
    for(int i=0;i<n;i++) b[i]=a[i];
    int *flags =random_data_gen<int>(n,bd);
    flags[0]=1;
    // TIME(exclusive_scan_seq(b,init,n,std::plus<int>()); )  
    TIME(exclusive_scan_par(a,init,n,std::plus<int>());)
    for(int i=0;i<n;i++){
        if(a[i]!=b[i]){
            std::cout<<"Error at index "<<i<<std::endl;
            std::cout<<"Expected "<<b[i]<<" but got "<<a[i]<<std::endl;
            return;
        }
    }
    std::cout<<"Test passed"<<std::endl;
    delete a,b,flags;
}