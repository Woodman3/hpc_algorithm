#include<cstddef>
#include<chrono>
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