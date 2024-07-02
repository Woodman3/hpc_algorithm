#include<algorithm>
#include<vector>
#include<numeric>
#include<iostream>
#include<execution>

using namespace std;

inline void permute(auto policy,vector<int> &a, vector<int> &index,vector<int> &seq_i) {
    size_t n = a.size();
    auto temp = vector<int>(n);
    for_each(policy,seq_i.begin(),seq_i.end(),[&](int i){temp[index[i]] = a[i];});
    for_each(policy,seq_i.begin(),seq_i.end(),[&](int i){a[i] = temp[i];});
}

vector<int> gen_seq_vec(size_t n){
    vector<int> a(n);
    iota(a.begin(),a.end(),0);
    return a;
}

/// if true go down else go up
void spilt(auto policy,vector<int> &a, vector<int> &flags,vector<int> &seq_i) {
    auto length = a.size();
    auto a_down = vector<int>(length);
    auto a_up = vector<int>(length);
    auto index = vector<int>(length);
    transform_exclusive_scan(policy,flags.begin(), flags.end(),a_down.begin(),0,plus<int>(),[](int x){return x?0:1;});
    exclusive_scan(policy,flags.rbegin(), flags.rend(),a_up.rbegin(),0);
    for_each(policy,seq_i.begin(),seq_i.end(),[&](int i){
        a_up[i] = length - a_up[i] - 1;
        index[i] = flags[i] ? a_up[i] : a_down[i];});
    permute(a, index,seq_i);
}

void radix_sort(vector<int>&a,int max){
    size_t n = a.size();
    vector<int> flags(n);
    auto seq_i = gen_seq_vec(n);
    for(int i=0;i<max;i++){
        for(int j=0;j<n;j++){
            flags[j] = a[j] & (1<<i);
        }
        spilt(execution::par_unseq,a,flags,seq_i);
    }
}

int main(){
    vector<int> temp = {1,2,3,4,5,6,7,8,9,10};
    vector<int> flags = {1,0,1,0,1,0,1,0,1,0};
    // par_spilt(temp, flags);
    for_each(temp.begin(),temp.end(),[](int x){cout<<x<<" ";});
    return 0;
}