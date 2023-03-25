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

string inputfile = "seed001-08in.txt";
string outputfile = "seed001-08out.txt";

struct vec3{
    int x, y, z;
    bool operator<(const vec3& v2) const{
        return x == v2.x ? (y == v2.y ? z<v2.z : y<v2.y) : x<v2.x;
    }
};

struct Solver{
    int d;
    vector<vector<vector<int>>> silhouette;
    vector<vector<vector<bool>>> isblock0, isblock1;
    vector<vector<vector<bool>>> canexist0, canexist1;
    vector<vector<vector<bool>>> shareable;
    vector<vector<int>> ans;
    vector<int> blocknum;
    Solver(int d, vector<vector<vector<int>>> silhouette) 
    : d(d), silhouette(silhouette), ans(vector<vector<int>>(2)), blocknum({0,0}), isblock0(d, vector<vector<bool>>(d, vector<bool>(d, false))), isblock1(d, vector<vector<bool>>(d, vector<bool>(d, false))), canexist0(d, vector<vector<bool>>(d, vector<bool>(d, false))), canexist1(d, vector<vector<bool>>(d, vector<bool>(d, false))), shareable(d, vector<vector<bool>>(d, vector<bool>(d, false))) { }

    void solve(){
        blockpos(0, isblock0);
        blockpos(1, isblock1);
        canexist0 = isblock0;
        canexist1 = isblock1;
        for(int x=0; x<d; x++){
            for(int y=0; y<d; y++){
                for(int z=0; z<d; z++){
                    shareable[x][y][z] = canexist0[x][y][z] & canexist1[x][y][z];
                    if(shareable[x][y][z]){
                        isblock0[x][y][z] = 1;
                        isblock1[x][y][z] = 1;
                    }
                    else{
                        isblock0[x][y][z] = 0;
                        isblock1[x][y][z] = 0;
                    }
                }
            }
        }
        
        // destruct(0, isblock0);
        // destruct(1, isblock1);
        // hideblock();

        assignblocknum(0, isblock0);
        assignblocknum(1, isblock1);

        ansout();
        return;
    }

    //ブロックを置けるかどうか
    void blockpos(int sil, vector<vector<vector<bool>>>& isblock){
        int f = sil*2, r = sil*2+1;
        for(int x=0; x<d; x++){
            for(int y=0; y<d; y++){
                for(int z=0; z<d; z++){
                    if(silhouette[f][z][x]*silhouette[r][z][y]){
                        isblock[x][y][z] = true;
                    }
                }
            }
        }
    }

    void ansout(){
///Local
        // ofstream FILEOUT(outputfile);
        // FILEOUT << max(blocknum[0], blocknum[1]) << endl;
        // for(int i=0; i<2; i++){
        //     for(auto b:ans[i]) FILEOUT << b << " ";
        //     FILEOUT << endl;
        // }
//Submit
        cout << max(blocknum[0], blocknum[1]) << endl;
        for(int i=0; i<2; i++){
            for(auto b:ans[i]) cout << b << " ";
            cout << endl;
        }

        return;
    }

    //不要なブロックを削る
    void destruct(int sil, vector<vector<vector<bool>>>& isblock){
        vector<vector<bool>> zy_projection(d, vector<bool>(d, false)), zx_projection(d, vector<bool>(d, false));
        int n=0;
        for(int x=0; x<d; x++){
            for(int y=0; y<d; y++){
                for(int z=0; z<d; z++){
                    if(isblock[x][y][z]){
                        if(zx_projection[z][x] && zy_projection[z][y]){
                            isblock[x][y][z] = false;
                            continue;
                        }

                        zx_projection[z][x] = true;
                        zy_projection[z][y] = true;
                        n++;
                    }
                }
            }
        }
        blocknum[sil] = n;
    }

    //使われていないブロックを隠して置く
    void hideblock(){
        int dist = blocknum[0] - blocknum[1];
        if(dist==0) return;

        //1のほうが多いので0に隠す
        else if(dist<0) hide(-dist, isblock0, canexist0);
        //0のほうが多いので1に隠す
        else hide(dist, isblock1, canexist1);
        return;
    }

    void hide(int dist, vector<vector<vector<bool>>>& isblock, vector<vector<vector<bool>>> canexist){
        for(int x=0; x<d; x++){
            for(int y=0; y<d; y++){
                for(int z=0; z<d; z++){
                    if(!isblock[x][y][z] && canexist[x][y][z]){
                        isblock[x][y][z] = true;
                        dist--;
                        if(dist==0) break;
                    }
                }
                if(dist==0) break;
            }
            if(dist==0) break;
        }
        return;
    }

    void assignblocknum(int sil, vector<vector<vector<bool>>> isblock){
        int n=0;
        for(int x=0; x<d; x++){
            for(int y=0; y<d; y++){
                for(int z=0; z<d; z++){
                    if(isblock[x][y][z]){
                        n++;
                        ans[sil].push_back(n);
                    }
                    else ans[sil].push_back(0);
                }
            }
        }
        blocknum[sil] = n;
    }
};


int main(){
    srand((unsigned int)time(NULL));
    chrono::system_clock::time_point  start, end; 
    start = chrono::system_clock::now(); // 計測開始時間

///Local
    // ifstream FILEIN(inputfile);
    // int d; FILEIN >> d;
    // vector<vector<vector<int>>> silhouette(4, vector<vector<int>>(d, vector<int>(d)));
    // for(int sil=0; sil<4; sil++){
    //     for(int i=0; i<d; i++){
    //         for(int j=0; j<d; j++){
    //             char a; FILEIN >> a;
    //             silhouette[sil][i][j] = a-'0';
    //         }
    //     }
    // }
    // Solver solver(d, silhouette);
    // solver.solve();

///Submit
    int d; cin >> d;
    vector<vector<vector<int>>> silhouette(4, vector<vector<int>>(d, vector<int>(d)));
    for(int sil=0; sil<4; sil++){
        for(int i=0; i<d; i++){
            for(int j=0; j<d; j++){
                char a; cin >> a;
                silhouette[sil][i][j] = a-'0';
            }
        }
    }
    Solver solver(d, silhouette);
    solver.solve();

    end = chrono::system_clock::now();  // 計測終了時間
    double elapsed = chrono::duration_cast<chrono::milliseconds>(end-start).count(); //処理に要した時間をミリ秒に変換
    // cout << "#finished" << endl;
    // cout << "#Time: " << elapsed << " ms" << endl;
    return 0;
}