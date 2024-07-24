#include<cstddef>
#include<chrono>
#include<iostream>
#include<omp.h>

#define TIME(f) { \
    auto start = std::chrono::high_resolution_clock::now(); \
    f; \
    auto end = std::chrono::high_resolution_clock::now(); \
    std::chrono::duration<double> elapsed = end - start; \
    std::cout << "Elapsed time: " << elapsed.count() << " s\n"; \
}


template<typename T, class OP>
void exclusive_scan_par(T* a,T init, size_t n, OP op){
    int offset=1;
    for(;offset<n;offset<<=1){
        #pragma omp parallel for
        #pragma unroll 
        for(int i =n-1-offset;i>=0;i-=offset<<1){
            a[i+offset] = op(a[i],a[i+offset]);
        }
    }
    a[n-1] = init;
    for(;offset>0;offset>>=1){
        #pragma omp parallel for
        #pragma unroll 
        for(int i =(n-1)-offset;i>=0;i-=offset<<1){
            T temp = a[i];
            a[i] = a[i+offset];
            a[i+offset] = op(a[i+offset],temp);
        }
    }
}

template<typename T, class OP>
void exclusive_scan_seq(T* a,T init, size_t n, OP op){
    T tmp = a[0];
    a[0] = init;
    for(int i=1;i<n;i++){
        T t = a[i];
        a[i] =op(tmp,a[i-1]);
        tmp = t;
    }
}

void show(int* a, size_t n){
    for(int i=0;i<n;i++){
        std::cout<<a[i]<<" ";
    }
    std::cout<<std::endl;
}

template<typename T, class OP>
void exclusive_seg_scan_seq(T* a,int* flags,T init, size_t n, OP op){
    // assert(flags[0]==1);
    T tmp = a[0];
    for(int i=0;i<n;i++){
        if(flags[i]){
            tmp = a[i];
            a[i] = init;
        }else{
            T t = a[i];
            a[i] =op(tmp,a[i-1]);
            tmp = t;
        }
    }
}

template<typename T, class OP>
void exclusive_seg_scan_par(T* a,int* flags,T init, size_t n, OP op){
    // assert(flags[0]==1);
    show(a,n);
    show(flags,n);
    int offset=1;
    int fb[n];
    for(int i=0;i<n;i++){
        fb[i] = flags[i];
    }
    for(;offset<n;offset<<=1){
        // #pragma omp parallel for
        // #pragma unroll 
        for(int i =n-1-offset;i>=0;i-=offset<<1){
            if(!flags[i+offset]){
                a[i+offset] = op(a[i],a[i+offset]);
                flags[i+offset] |= flags[i];
            }
        }
        show(a,n);
    }
    a[n-1] = init;
    flags[n-1]=0;
    std::cout<<"down"<<std::endl;
    for(;offset>0;offset>>=1){
        // #pragma omp parallel for
        // #pragma unroll 
        for(int i =(n-1)-offset;i>=0;i-=offset<<1){
            T temp = a[i];
            a[i]= a[i+offset];
            if(fb[i+1]){
                a[i+offset] = init;
            }
            else if(flags[i]){
                a[i+offset] = op(init,temp);
            }else{
                a[i+offset] = op(a[i+offset],temp);
            }
        }
        show(a,n);
    }
}