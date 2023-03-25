#include <iostream>
#include <vector>
using namespace std;

int main(){
    vector<int> a;
    a.push_back(100);
    // a.push_back(5);
    // a.pop_back();
    a[0] = 9;
    for(auto v:a) cout << v << endl;
    return 0;
}