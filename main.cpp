#include "scan.hpp"
#include <iostream>
#include <cstring>
#include <random>

using namespace std;

int main(){
    std::random_device r;
    std::default_random_engine e(r());
    std::uniform_int_distribution<int>dis(0,4);
    std::bernoulli_distribution bd(0.8);
    int n=10;
    int init =2;
    int *a= new int[n];
    int *b=new int[n];
    int *flags =new int[n];
    for(int i=0;i<n;i++){
        a[i]=dis(e);
        b[i]=a[i];
        flags[i] =bd(e)?0:1;
    }
    flags[n-1]=1;
    TIME(exclusive_seg_scan_seq_reverse(b,flags,init,n,plus<int>()); )  
    TIME(exclusive_seg_scan_par_reverse(a,flags,init,n,plus<int>());)
    // TIME(exclusive_scan_seq_reverse(b,init,n,plus<int>()); )  
    // TIME(exclusive_scan_par_reverse(a,init,n,plus<int>());)
    for(int i=0;i<n;i++){
        if(a[i]!=b[i]){
            cout<<"Error at index "<<i<<endl;
            cout<<"Expected "<<b[i]<<" but got "<<a[i]<<endl;
            return -1;
        }
    }
    return 0;
}
