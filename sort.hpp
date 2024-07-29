#include <functional>
#include "scan.hpp"
#include "test.hpp"

template<typename T>
inline void permute(T* a,int* index,T* work,int n) {
    #pragma omp parallel for
    #pragma unroll 
    for(int i=0;i<n;i++)
        work[index[i]] = a[i];
    #pragma omp parallel for
    #pragma unroll 
    for(int i=0;i<n;i++)
        a[i]=work[i];

}
/// if true go down else go up
template<typename T>
void spilt(T* a, int* flags,int* a_down,int* a_up,T* index,int n) {
    #pragma omp parallel for
    #pragma unroll 
    for(int i=0;i<n;i++){
        if(flags[i]){
            flags[i]=1;
            a_up[i]=1;
            a_down[i]=0;
        }else{
            flags[i]=0;
            a_up[i]=0;
            a_down[i]=1;
        }
    }
    exclusive_scan_par(a_down,0,n,std::plus<int>());
    exclusive_scan_par_reverse(a_up,0,n,std::plus<int>());
    #pragma omp parallel for
    #pragma unroll 
    for(int i=0;i<n;i++){
        a_up[i]=n-a_up[i]-1;
        index[i] = flags[i]?a_up[i]:a_down[i];
    }
    // use a_up as workspace
    permute(a, index,a_up,n);
}

template<typename T>
void radix_sort(T* a,int max_power,int n){
    int *a_up = new int[n];
    int *a_down = new int[n];
    int *flags = new int[n];
    int *index = new int[n];
    for(int i=0;i<=max_power;i++){
        #pragma omp parallel for
        #pragma unroll 
        for(int j=0;j<n;j++){
            flags[j] = a[j] & (1<<i);
        }
        spilt(a,flags,a_up,a_down,index,n);
    }
    delete a_up,a_down,flags,index;
}

template<typename T>
void quick_sort(T*a,int n){

}