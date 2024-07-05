#include "scan.hpp"
#include <iostream>
#include <cstring>

using namespace std;

void add(int *a,int n){
    for(int i=0;i<n;i++){
        a[i]++;
    }
}

int main(){
    int n=8;
    int init =0;
    int *a= new int[n];
    int *b=new int[n];
    for(int i=0;i<n;i++){
        a[i]=i;
        b[i]=i;
    }
    exclusive_scan_par(a,init,n,plus<int>());
    exclusive_scan_seq(b,init,n,plus<int>());  
    for(int i=0;i<n;i++){
        if(a[i]!=b[i]){
            cout<<"Error at index "<<i<<endl;
            cout<<"Expected "<<b[i]<<" but got "<<a[i]<<endl;
        }
    }
    return 0;
}
