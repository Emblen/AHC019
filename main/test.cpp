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
    int r, c;
    cin >> r >> c;
    v2i dotmap(v2i(r, vector<int>(c, 0)));
    for(int i=0; i<r; i++){
        for(int j=0; j<c; j++){
            cin >> dotmap[i][j];
        }
    }
    v2i dotmap2(v2i(c, vector<int>(r, 0)));
    for(int i=0; i<c; i++){
        for(int j=0; j<r; j++){
            cin >> dotmap2[i][j];
        }
    }

    for(int i=0; i<r; i++){
        dotmap[i].erase(all(dotmap[i]));
        dotmap[i].shrink_to_fit();
    }
    dotmap.erase(all(dotmap));
    dotmap.shrink_to_fit();

    // cout << dotmap.size() << endl;
    dotmap = dotmap2;

    for(int i=0; i<c; i++){
        for(int j=0; j<r; j++) cout << dotmap[i][j] << " ";
        cout << endl;
    }
    
    return 0;
}