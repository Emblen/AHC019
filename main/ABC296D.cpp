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
    long long n, m; cin>>n>>m;
    if(n<sqrt(m)){
        cout << -1 << endl;
        return 0;
    }
    for(long long x=m; x<=n*n; x++){
        long long a = sqrt(x);
        while(a<=n){
            if(x%a==0 && x/a<=n){
                cout << x << endl;
                return 0;
            }
            a++;
        }
    }
    
    return 0;
}