#pragma once
#include<cstddef>
#include<chrono>
#include<iostream>
#include<omp.h>


template<typename T, class OP>
void exclusive_scan_par(T* a,T init, size_t n, OP op){
    int offset=1;
    for(;offset<(n>>1);offset<<=1){
        #pragma omp parallel for
        #pragma unroll 
        for(int i =n-1-( offset*3 );i>=0;i-=offset<<1){
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
void exclusive_scan_par_reverse(T* a,T init, size_t n, OP op){
    int offset=1;
    for(;offset<(n>>1);offset<<=1){
        #pragma omp parallel for
        #pragma unroll 
        for(int i =offset;i<n;i+=offset<<1){
            a[i-offset] = op(a[i],a[i-offset]);
        }
    }
    a[0] = init;
    for(;offset>0;offset>>=1){
        #pragma omp parallel for
        #pragma unroll 
        for(int i =offset;i<n;i+=offset<<1){
            T temp = a[i];
            a[i] = a[i-offset];
            a[i-offset] = op(a[i-offset],temp);
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

template<typename T, class OP>
void exclusive_scan_seq_reverse(T* a,T init, size_t n, OP op){
    T tmp = a[n-1];
    a[n-1] = init;
    for(int i=n-2;i>=0;i--){
        T t = a[i];
        a[i] =op(tmp,a[i+1]);
        tmp = t;
    }
}


template<typename T, class OP>
void exclusive_seg_scan_seq(T* a,int* flags,T init, size_t n, OP op){
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
    int offset=1;
    int fb[n];
    for(int i=0;i<n;i++){
        fb[i] = flags[i];
    }
    for(;offset<( n>>1 );offset<<=1){
        #pragma omp parallel for
        #pragma unroll 
        for(int i =n-1- ( offset*3 );i>=0;i-=offset<<1){
            if(!flags[i+offset]){
                a[i+offset] = op(a[i],a[i+offset]);
                flags[i+offset] |= flags[i];
            }
        }
    }
    a[n-1] = init;
    flags[n-1]=0;
    for(;offset>0;offset>>=1){
        #pragma omp parallel for
        #pragma unroll 
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
    }
}


template<typename T, class OP>
void exclusive_seg_scan_seq_reverse(T* a,int* flags,T init, size_t n, OP op){
    T tmp = a[n-1];
    for(int i=n-1;i>=0;i--){
        if(flags[i]){
            tmp = a[i];
            a[i] = init;
        }else{
            T t = a[i];
            a[i] =op(tmp,a[i+1]);
            tmp = t;
        }
    }
}

template<typename T, class OP>
void exclusive_seg_scan_par_reverse(T* a,int* flags,T init, size_t n, OP op){
    int offset=1;
    int fb[n];
    for(int i=0;i<n;i++){
        fb[i] = flags[i];
    }
    for(;offset<( n>>1 );offset<<=1){
        #pragma omp parallel for
        #pragma unroll 
        for(int i =offset;i<n;i+=offset<<1){
            if(!flags[i-offset]){
                a[i-offset] = op(a[i],a[i-offset]);
                flags[i-offset] |= flags[i];
            }
        }
    }
    a[0] = init;
    flags[0]=0;
    for(;offset>0;offset>>=1){
        #pragma omp parallel for
        #pragma unroll 
        for(int i =offset;i<n;i+=offset<<1){
            T temp = a[i];
            a[i]= a[i-offset];
            if(fb[i-1]){
                a[i-offset] = init;
            }
            else if(flags[i]){
                a[i-offset] = op(init,temp);
            }else{
                a[i-offset] = op(a[i-offset],temp);
            }
        }
    }
}