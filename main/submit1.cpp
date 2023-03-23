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
#define INF 1e6

using namespace std;

string inputfile = "seed000-05in.txt";
string outputfile = "seed000-05out.txt";

struct vec3{
    int x, y, z;
    bool operator<(const vec3& v2) const{
        return x == v2.x ? (y == v2.y ? z<v2.z : y<v2.y) : x<v2.x;
    }
};

struct Solver{
    int d;
    vector<vector<vector<int>>> silhouette;
    vector<vector<vector<bool>>> isblock;
    vector<vector<int>> ans;
    vector<int> blocknum;
    Solver(int d, vector<vector<vector<int>>> silhouette) 
    : d(d), silhouette(silhouette), ans(vector<vector<int>>(2)), blocknum({0,0}), isblock(d, vector<vector<bool>>(d, vector<bool>(d, false))) { }

    void solve(){
        for(int sil=0; sil<2; sil++){
            int n=1;
            int f = sil*2, r = sil*2+1;
            for(int x=0; x<d; x++){
                for(int y=0; y<d; y++){
                    for(int z=0; z<d; z++){
                        if(silhouette[f][z][x]*silhouette[r][z][y]){
                            ans[sil].push_back(n);
                            n++;
                        }
                        else ans[sil].push_back(0);
                    }
                }
            }
            blocknum[sil] = n;
        }
        ansout();
        return;
    }

    void ansout(){
///Local
        ofstream FILEOUT(outputfile);
        FILEOUT << max(blocknum[0], blocknum[1]) << endl;
        for(int i=0; i<2; i++){
            for(auto b:ans[i]) FILEOUT << b << " ";
            FILEOUT << endl;
        }
//Submit
        // cout << max(blocknum[0], blocknum[1]) << endl;
        // for(int i=0; i<2; i++){
        //     for(auto b:ans[i]) FILEOUT << b << " ";
        //     cout << endl;
        // }

        return;
    }

    void destruct(){
        vector<vector<bool>> zy_projection(d, vector<bool>(d, false)), zx_projectioin(d, vector<bool>(d, false));
        for(int sil=0; sil<2; sil++){
            
        }
    }

};


int main(){
    srand((unsigned int)time(NULL));
    chrono::system_clock::time_point  start, end; 
    start = chrono::system_clock::now(); // 計測開始時間

///Local
    ifstream FILEIN(inputfile);
    int d; FILEIN >> d;
    vector<vector<vector<int>>> silhouette(4, vector<vector<int>>(d, vector<int>(d)));
    for(int sil=0; sil<4; sil++){
        for(int i=0; i<d; i++){
            for(int j=0; j<d; j++){
                char a; FILEIN >> a;
                silhouette[sil][i][j] = a-'0';
            }
        }
    }
    Solver solver(d, silhouette);
    solver.solve();

///Submit
    // int d; cin >> d;
    // vector<vector<vector<int>>> silhouette(4, vector<vector<int>>(d, vector<int>(d)));
    // for(int sil=0; sil<4; sil++){
    //     for(int i=0; i<d; i++){
    //         for(int j=0; j<d; j++){
    //             char a; cin >> a;
    //             silhouette[sil][i][j] = a-'0';
    //         }
    //     }
    // }

    cout << "#finished" << endl;
    end = chrono::system_clock::now();  // 計測終了時間
    double elapsed = chrono::duration_cast<chrono::milliseconds>(end-start).count(); //処理に要した時間をミリ秒に変換
    cout << "#Time: " << elapsed << " ms" << endl;
    return 0;
}