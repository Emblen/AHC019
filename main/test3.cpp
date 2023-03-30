#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main(){
    vector<vector<vector<int>>> input1 = {{{1,0},{1,1}},{{0,0},{1,0}}};
    vector<vector<vector<int>>> input2 = {{{1,1},{0,1}},{{1,1},{0,0}}};

    for(int x=0; x<2; x++){
        for(int y=0; y<2; y++){
            for(int z=0; z<2; z++){
                cout << input2[x][y][z] << " ";
            }
            cout<<endl;
        }
        cout << endl;
    }
    input2 = input1;
    for(int x=0; x<2; x++){
        for(int y=0; y<2; y++){
            for(int z=0; z<2; z++){
                cout << input2[x][y][z] << " ";
            }
            cout<<endl;
        }
        cout << endl;
    }
    return 0;
}