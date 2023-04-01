#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <set>
#include <queue>
#include <map>
using namespace std;

int main(){
    int n, x; cin>>n>>x;
    vector<int> a(n);
    for(int i=0; i<n; i++) cin>>a[i];
    sort(a.begin(), a.end());

    int left=0, right=0;
    while(left<n){
        int diff = a[right]-a[left];
        if(diff==x){
            cout << "Yes" << endl;
            return 0;
        }
        else if(diff<x){
            right++;
            continue;
        }
        else left++;
    }
    cout << "No" << endl;
    return 0;
}