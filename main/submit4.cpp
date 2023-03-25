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

using namespace std;

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
    v3i silhouette, ans0, ans1;
    v3b isblock0, isblock1, canexist0, canexist1, isCommon;
    v2i ans;
    v2b silf0, silr0, silf1, silr1;
    vector<Block> blockset0, blockset1;

    Solver(int d, v3i silhouette) 
    : d(d), silhouette(silhouette), isblock0(init_false_v3b_d), isblock1(init_false_v3b_d), canexist0(init_false_v3b_d), canexist1(init_false_v3b_d), silf0(init_false_v2b_d), silf1(init_false_v2b_d), silr0(init_false_v2b_d), silr1(init_false_v2b_d), common_bfs(d), ans0(init_zero_v3i_d), ans1(init_zero_v3i_d) { }

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
        blockset0 = common_bfs.blockset;
        blockset1 = common_bfs.blockset;

        v3b isblock0(init_false_v3b_d);
        v3b isblock1(init_false_v3b_d);
        //これまでに構成したブロックを投影する
        projectioin(silf0, silr0, isblock0, blockset0);
        projectioin(silf1, silr1, isblock1, blockset1);

        monocube(silf0, silr0, canexist0, isblock0, blockset0);
        monocube(silf1, silr1, canexist1, isblock1, blockset1);

        assignblocknum(ans0, blockset0);
        assignblocknum(ans1, blockset1);

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

    void projectioin(v2b& front, v2b& right, v3b& isblock, vector<Block> blockset){
        for(auto block:blockset){
            auto [blocksize, cubevec] = block;
            for(auto cube:cubevec){
                auto [cx, cy, cz] = cube;
                isblock[cx][cy][cz] = true;
                front[cz][cx] = true;
                right[cz][cy] = true;
            }
        }
    }

    //シルエットが埋まっていない位置に、モノキューブを置いていく。blocksetに追加していく。
    void monocube(v2b& front, v2b& right, v3b canexist, v3b& isblock, vector<Block>& blockset){
        for(int z=0; z<d; z++){
            int unplaced = 0;
            for(int x=0; x<d; x++){
                for(int y=0; y<d; y++){
                    if(canexist[x][y][z] && !isblock[x][y][z]){//モノキューブを置けるが置かれていない場所のみに置く
                        if(!front[z][x] && !right[z][y]){//両方のシルエットが空いていたらキューブを追加する
                            isblock[x][y][z] = true;
                            blockset.push_back({1, {{x,y,z}}});
                            front[z][x] = true;
                            right[z][y] = true;
                        }
                        else if(!front[z][x] || !right[z][y]){//片方のシルエットが空いていたら数をカウントしてスキップ
                            unplaced++;
                        }
                    }
                }
            }
            for(int x=0; x<d; x++){
                for(int y=0; y<d; y++){
                    if(canexist[x][y][z] && !isblock[x][y][z] && (!front[z][x] || !right[z][y])){
                        isblock[x][y][z] = true;
                        blockset.push_back({1, {{x,y,z}}});
                        front[z][x] = true;
                        right[z][y] = true;
                        unplaced--;
                        if(unplaced==0) break;
                    }
                }
                if(unplaced==0) break;
            }
        }
    }

    void assignblocknum(v3i& ans, vector<Block> blockset){
        int num = 0;
        for(auto block:blockset){
            num++;
            auto [blocksize, cubevec] = block;
            for(auto cube:cubevec){
                auto [cx, cy, cz] = cube;
                ans[cx][cy][cz] = num;
            }
        }
    }

    void ansout(){
        cout << max(blockset0.size(), blockset1.size()) << endl;

        for(int x=0; x<d; x++){
            for(int y=0; y<d; y++){
                for(int z=0; z<d; z++){
                    cout << ans0[x][y][z] << " ";
                }
            }
        }
        cout << endl;
        for(int x=0; x<d; x++){
            for(int y=0; y<d; y++){
                for(int z=0; z<d; z++){
                    cout << ans1[x][y][z] << " ";
                }
            }
        }
        return;
    }
};


int main(){
    srand((unsigned int)time(NULL));
    chrono::system_clock::time_point  start, end; 
    start = chrono::system_clock::now(); // 計測開始時間

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