#include "scan.hpp"
#include <iostream>
#include <cstring>
#include <random>

using namespace std;

int main(){
    std::random_device r;
    std::default_random_engine e(r());
    std::uniform_int_distribution<int>dis(-4,4);
    int n=100000000;
    int init =0;
    int *a= new int[n];
    int *b=new int[n];
    for(int i=0;i<n;i++){
        a[i]=dis(e);
        b[i]=a[i];
    }
    TIME(exclusive_scan_par(a,init,n,plus<int>());)
    TIME(exclusive_scan_seq(b,init,n,plus<int>()); )  
    for(int i=0;i<n;i++){
        if(a[i]!=b[i]){
            cout<<"Error at index "<<i<<endl;
            cout<<"Expected "<<b[i]<<" but got "<<a[i]<<endl;
        }
    }
    return 0;
}
