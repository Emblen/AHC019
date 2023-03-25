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
#define init_false_v2b_d d, vector<bool>(d, false)
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

struct Block{
    int siz;
    vector<vec3> cooset;
};

struct BFS{
    int d;
    vector<vec3> dxdydz;
    vector<Block> blockset;
    v3b visit;

    BFS(int d)
    : d(d), visit(init_false_v3b_d), dxdydz({{0,0,1}, {0,0,-1}, {0,1,0}, {0,-1,0}, {1,0,0}, {-1,0,0}}) { }

    void bfs(v3b& isCommon){
        int setnumber = 0;
        for(int x=0; x<d; x++){
            for(int y=0; y<d; y++){
                for(int z=0; z<d; z++){
                    if(!isCommon[x][y][z] || visit[x][y][z]) continue;
                    
                    int presetnumber = setnumber;

                    queue<vec3> q3;
                    q3.push({x, y, z});
                    int blocksize = 0;
                    blockset.push_back({blocksize, {}});

                    while(!q3.empty()){
                        auto [pvx, pvy, pvz] = q3.front();
                        q3.pop();
                        if(visit[pvx][pvy][pvz]) continue;

                        visit[pvx][pvy][pvz] = true;
                        blockset[setnumber].cooset.push_back({pvx, pvy, pvz});
                        blocksize++;
                        
                        for(auto nv : dxdydz){
                            int nvx = pvx + nv.x;
                            int nvy = pvy + nv.y;
                            int nvz = pvz + nv.z;
                            if(nvx<0 || d<=nvx || nvy<0 || d<=nvy || nvz<0 || d<=nvz || !isCommon[nvx][nvy][nvz] || visit[nvx][nvy][nvz]) continue;

                            q3.push({nvx, nvy, nvz});
                        }

                        if(blocksize==1 && q3.size()==0){
                            auto [popx, popy, popz] = blockset[setnumber].cooset[0];
                            visit[popx][popy][popz] = false;
                            blockset.pop_back(); //モノキューブだけならブロックを削除
                            setnumber--;
                            break;
                        }
                    }
                    if(presetnumber==setnumber) blockset[setnumber].siz = blocksize; //ブロックサイズの更新
                    setnumber++;
                }
            }
        }
    }
};

struct Solver{
    int d;
    BFS common_bfs;
    v3i silhouette;
    v3b isblock0, isblock1, canexist0, canexist1, isCommon;
    v2i ans;
    v2b silf1, silr1, silf2, silr2;
    vector<int> blocknum;
    vector<Block> blockset;
    Solver(int d, v3i silhouette) 
    : d(d), silhouette(silhouette), ans(v2i(2)), blocknum({0,0}), isblock0(init_false_v3b_d), isblock1(init_false_v3b_d), canexist0(init_false_v3b_d), canexist1(init_false_v3b_d), silf1(init_false_v2b_d), silf2(init_false_v2b_d), silr1(init_false_v2b_d), silr2(init_false_v2b_d), common_bfs(d) { }

    void solve(){
        blockpos(0, isblock0);
        blockpos(1, isblock1);
        canexist0 = isblock0;
        canexist1 = isblock1;

        //同じ座標にモノキューブがあるかどうか
        for(int x=0; x<d; x++){
            for(int y=0; y<d; y++){
                for(int z=0; z<d; z++){
                    if(canexist0[x][y][z] && canexist1[x][y][z]){
                        isblock0[x][y][z] = true;
                        isblock1[x][y][z] = true;
                    }
                    else{
                        isblock0[x][y][z] = false;
                        isblock1[x][y][z] = false;
                    }
                }
            }
        }
        common_bfs.bfs(isblock0);
        blockset = common_bfs.blockset;
        
        //これまでに構成したブロックを投影する
        for(auto block:blockset){
            auto [blocksize, cubevec] = block;
            for(auto cube:cubevec){
                auto [cx, cy, cz] = cube;
                silf1[cz][cx] = true;
                silf2[cz][cx] = true;
                silr1[cz][cy] = true;
                silr2[cz][cy] = true;
            }
        }

        // destruct(0, isblock0, canexist0);
        // destruct(1, isblock1, canexist1);
        // hideblock();

        assignblocknum(0, isblock0);
        assignblocknum(1, isblock1);

        ansout();
        return;
    }

    //ブロックを置けるかどうか
    void blockpos(int sil, v3b& isblock){
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
    void destruct(int sil, v3b& isblock, v3b canexist){
        v2b zy_projection(d, vector<bool>(d, false)), zx_projection(d, vector<bool>(d, false));
        int n=0;
        for(int z=0; z<d; z++){
            int unplaced = 0;
            for(int x=0; x<d; x++){
                for(int y=0; y<d; y++){
                    if(isblock[x][y][z]){
                        if(zx_projection[z][x] && zy_projection[z][y]){
                            isblock[x][y][z] = false;
                        }
                        else if(zx_projection[z][x] || zy_projection[z][y]){
                            isblock[x][y][z] = false;
                            unplaced++;
                        }
                        else{
                            zx_projection[z][x] = true;
                            zy_projection[z][y] = true;
                            n++;
                        }
                    }
                }
            }
            for(int x=0; x<d; x++){
                for(int y=0; y<d; y++){
                    if(canexist[x][y][z] && !isblock[x][y][z] && (!zx_projection[z][x] || !zy_projection[z][y])){
                        isblock[x][y][z] = true;
                        zx_projection[z][x] = true;
                        zy_projection[z][y] = true;
                        unplaced--;
                        n++;
                        if(unplaced==0) break;
                    }
                }
                if(unplaced==0) break;
            }
            
        }
        blocknum[sil] = n;
    }

    //使われていないブロックを隠して置く（単位ブロックのときは得点に影響しない）
    void hideblock(){
        int dist = blocknum[0] - blocknum[1];
        if(dist==0) return;

        //1のほうが多いので0に隠す
        else if(dist<0) hide(-dist, isblock0, canexist0);
        //0のほうが多いので1に隠す
        else hide(dist, isblock1, canexist1);
        return;
    }

    void hide(int dist, v3b& isblock, v3b canexist){
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

    //シルエットが埋まっていない位置に、モノキューブを置いていく。blocksetに追加していく。
    void destruct(int sil, v2b front, v2b right,  v3b canexist){
        

    }


    void assignblocknum(int sil, v3b isblock){
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
    v3i silhouette(4, v2i(d, vector<int>(d)));
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