#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main(){
    for(int i=0; i<64; i++){
        char c; cin>>c;
        if(c=='*'){
            cout << (char)(i%8+97) << 8-i/8 << endl;
            return 0;
        }
    }
}