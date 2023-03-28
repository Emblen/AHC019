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

struct vec3{
    int x, y, z;
};

struct Polyomino{
    int r, c;
    v2i ismonomino;
    Polyomino(int r, int c, v2i ismonomino)
    : r(r), c(c), ismonomino(ismonomino) { }

    void lotate90(){
        v2i afterlotation(v2i(c, vector<int>(r,0)));
        for(int i=0; i<r; i++){
            for(int j=0; j<c; j++){
                int ny = j;
                int nx = -i+r-1;
                afterlotation[ny][nx] = ismonomino[i][j];
            }
        }
        for(int i=0; i<r; i++){
            ismonomino[i].erase(all(ismonomino[i]));
            ismonomino[i].shrink_to_fit();
        }
        ismonomino.erase(all(ismonomino));
        ismonomino.shrink_to_fit();
        
        int tmp = r; r = c; c = tmp;
        ismonomino = afterlotation;
        // cout << ismonomino.capacity() << " " << ismonomino[0].capacity() << endl;
    }
    
    void coutismonomino(){
        cout << r << " " << c << endl;
        for(int i=0; i<r; i++){
            for(int j=0; j<c; j++) cout << ismonomino[i][j] << " ";
            cout << endl;
        }
    }
};

int main(){
    chrono::system_clock::time_point  start, end; 
    start = chrono::system_clock::now(); // 計測開始時間

    int r, c;
    v2i dotmap(v2i(r, vector<int>(c, 0)));
    dotmap = {{1, 1, 1, 1},
              {1, 1, 1, 1},
              {1, 1, 1, 1}};
    r = dotmap.size();
    c = dotmap[0].size();

    int n = 11;
    v2i cmp =  {{0,0,0,0,0,0,0,0,1,0,0},
                {0,0,0,0,0,0,0,0,1,0,1},
                {0,0,0,0,0,0,0,0,1,1,1},
                {0,0,0,0,0,0,1,1,1,1,1},
                {0,0,0,0,0,0,1,0,1,1,1},
                {0,0,1,1,0,0,1,1,1,1,1},
                {0,0,0,1,1,1,1,0,0,0,1},
                {0,0,0,0,1,0,0,1,0,0,1},
                {0,0,1,1,0,0,1,1,1,1,1},
                {0,0,0,1,1,1,1,1,1,1,1},
                {0,0,1,1,1,1,1,1,1,1,1}};
    Polyomino polyomino(r, c, dotmap);
    int ans = 0;
    for(int p=0; p<4; p++){
        v2i tmpomino = polyomino.ismonomino;
        r = polyomino.r;
        c = polyomino.c;
        cout << endl;
        cout << "p=" << p << endl;
        polyomino.coutismonomino();
        for(int i=0; i<n-r+1; i++){
            for(int j=0; j<n-c+1; j++){
                bool tmp = true;
                for(int k=0; k<r; k++){
                    for(int l=0; l<c; l++){
                        if(tmpomino[k][l]==1 && cmp[i+k][j+l]==0) tmp=false;
                        if(!tmp) break;
                    }
                    if(!tmp) break;
                }
                if(tmp) ans++;
            }
        }
        polyomino.lotate90();
        cout << "ans: " << ans << endl;
    }
    
    end = chrono::system_clock::now();  // 計測終了時間
    double elapsed = chrono::duration_cast<chrono::milliseconds>(end-start).count(); //処理に要した時間をミリ秒に変換
    cout << "#Time: " << elapsed << " ms" << endl;
    return 0;
}  