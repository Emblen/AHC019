#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <set>
#include <queue>
#include <tuple>
#include <random>
#include <time.h>
#include <chrono>
using namespace std;
#define all(x) (x).begin(),(x).end()
#define v3i vector<vector<vector<int>>>
#define v2i vector<vector<int>>
#define v3b vector<vector<vector<bool>>>
#define v2b vector<vector<bool>>
#define init_false_v3b_d d,vector<vector<bool>>(d, vector<bool>(d, false))
#define init_zero_v3i_d d,vector<vector<int>>(d, vector<int>(d, 0))
#define init_false_v2b_d d,vector<bool>(d, false)
#define INF 1e6

int main(){
    int n; cin>>n;
    vector<int> a(n);
    for(int i=0; i<n; i++) cin>>a[i];
    int n2; cin>>n2;
    vector<int> b(n2);
    for(int i=0; i<n2; i++) cin>>b[i];
    
    vector<int> c;
    c = a;
    c.clear();
    c.shrink_to_fit();
    c = b;
    for(auto v:c) cout << v << " ";
    cout << endl;
    cout << c.size() << " " << c.capacity() << endl;
    return 0;
}