#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main(){
    int n; cin>>n;
    string s; cin>>s;
    if(s.length()==1){
        cout << "Yes" << endl;
        return 0;
    }
    for(int i=0; i<s.length()-1; i++){
        if(s[i]==s[i+1]){
            cout << "No" << endl;
            return 0;
        }
    }
    cout << "Yes" << endl;
    return 0;
}