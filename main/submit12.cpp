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
#include <map>
using namespace std;
#define all(x) (x).begin(),(x).end()
#define v3i vector<vector<vector<int>>>
#define v2i vector<vector<int>>
#define v3b vector<vector<vector<bool>>>
#define v2b vector<vector<bool>>
#define init_false_v3b_d d,vector<vector<bool>>(d, vector<bool>(d, false))
#define init_zero_v3i_d d,vector<vector<int>>(d, vector<int>(d, 0))
#define init_false_v2b_d d,vector<bool>(d, false)
#define init_zero_v2i_d d,vector<int>(d, 0)
#define INF 1e6

using namespace std;

struct vec3{
    int x, y, z;
    bool operator<(const vec3& v2) const{
        return x == v2.x ? (y == v2.y ? z<v2.z : y<v2.y) : x<v2.x;
    }
};

struct Block{
    int num;
    int siz;
    vector<vec3> cooset;
};

struct Lotation{
    int d;
    Lotation(int d)
    : d(d) { }

    void lotate90(int mode, v3b& beforelotation, vector<vector<vector<vec3>>>& originalcoo){

        v3b afterlotation(init_false_v3b_d);
        vector<vector<vector<vec3>>> tmporiginal(d, vector<vector<vec3>>(d, vector<vec3>(d)));

        if(mode==1){//x軸まわり
            for(int x=0; x<d; x++){
                for(int y=0; y<d; y++){
                    for(int z=0; z<d; z++){
                        int nx = x;
                        int ny = -z+d-1;
                        int nz = y;
                        afterlotation[nx][ny][nz] = beforelotation[x][y][z];
                        tmporiginal[nx][ny][nz] = originalcoo[x][y][z];
                    }
                }
            }
        }
        else if(mode==2){//y軸まわり
            for(int x=0; x<d; x++){
                for(int y=0; y<d; y++){
                    for(int z=0; z<d; z++){
                        int nx = z;
                        int ny = y;
                        int nz = -x+d-1;
                        afterlotation[nx][ny][nz] = beforelotation[x][y][z];
                        tmporiginal[nx][ny][nz] = originalcoo[x][y][z];
                    }
                }
            }
        }
        else if(mode==3){//z軸まわり
            for(int x=0; x<d; x++){
                for(int y=0; y<d; y++){
                    for(int z=0; z<d; z++){
                        int nx = -y+d-1;
                        int ny = x;
                        int nz = z;
                        afterlotation[nx][ny][nz] = beforelotation[x][y][z];
                        tmporiginal[nx][ny][nz] = originalcoo[x][y][z];
                    }
                }
            }
        }
        else{
            tmporiginal = originalcoo;
            afterlotation = beforelotation;
        }

        originalcoo = tmporiginal;
        beforelotation = afterlotation;
    }
};

struct BFS{
    int d;
    vector<vec3> dxdydz;
    v3b visit;

    BFS(int d)
    : d(d), visit(init_false_v3b_d), dxdydz({{0,0,1}, {0,0,-1}, {0,1,0}, {0,-1,0}, {1,0,0}, {-1,0,0}}) { }

    void bfs(v3b& isCommon, vector<Block>& blockset, int nowblocknum){
        int setnumber = nowblocknum;
        for(int x=0; x<d; x++){
            for(int y=0; y<d; y++){
                for(int z=0; z<d; z++){
                    if(!isCommon[x][y][z] || visit[x][y][z]) continue;
                    
                    int presetnumber = setnumber;

                    queue<vec3> q3;
                    q3.push({x, y, z});
                    int blocksize = 0;
                    blockset.push_back({setnumber+1, blocksize, {}});

                    while(!q3.empty()){
                        auto [pvx, pvy, pvz] = q3.front();
                        q3.pop();
                        if(visit[pvx][pvy][pvz]) continue;

                        visit[pvx][pvy][pvz] = true;
                        blockset[setnumber-nowblocknum].cooset.push_back({pvx, pvy, pvz});
                        blocksize++;
                        
                        for(auto nv : dxdydz){
                            int nvx = pvx + nv.x;
                            int nvy = pvy + nv.y;
                            int nvz = pvz + nv.z;
                            if(nvx<0 || d<=nvx || nvy<0 || d<=nvy || nvz<0 || d<=nvz || !isCommon[nvx][nvy][nvz] || visit[nvx][nvy][nvz]) continue;

                            q3.push({nvx, nvy, nvz});
                        }

                        if(blocksize<=2 && q3.size()==0){
                            for(auto block:blockset[setnumber-nowblocknum].cooset){
                                auto [popx, popy, popz] = block;
                                visit[popx][popy][popz] = false;
                            }
                            blockset.pop_back(); //diキューブ以下ならブロックを削除
                            setnumber--;
                            break;
                        }
                    }
                    if(presetnumber==setnumber) blockset[setnumber-nowblocknum].siz = blocksize; //ブロックサイズの更新
                    setnumber++;
                }
            }
        }
    }
};

struct Solver{
    int d;
    vector<int> modevec;
    int startmode2;
    BFS common_bfs;
    v3i silhouette, ans0, ans1;
    v3b isblock0, isblock1, canexist0, canexist1, isCommon;
    v2i ans;
    v2b silf0, silr0, silf1, silr1;
    int nowblocknum0, nowblocknum1;
    vector<Block> blockset0, blockset1;
    vector<vec3> dxdydz;
    map<int, pair<int, int>> blockinfo;
    Lotation lotation;
    vector<vector<vector<vec3>>> originalcoo;

    Solver(int d, v3i silhouette, vector<int> modevec, int startmode2) 
    : d(d), silhouette(silhouette), isblock0(init_false_v3b_d), isblock1(init_false_v3b_d), canexist0(init_false_v3b_d), canexist1(init_false_v3b_d), silf0(init_false_v2b_d), silf1(init_false_v2b_d), silr0(init_false_v2b_d), silr1(init_false_v2b_d), common_bfs(d), ans0(init_zero_v3i_d), ans1(init_zero_v3i_d), nowblocknum0(0), nowblocknum1(0), dxdydz({{0,0,1}, {0,0,-1}, {0,1,0}, {0,-1,0}, {1,0,0}, {-1,0,0}}), lotation(d), modevec(modevec), originalcoo(d, vector<vector<vec3>>(d, vector<vec3>(d))), startmode2(startmode2) { }

    void solve(){
        blockpos(0, canexist0);
        blockpos(1, canexist1);
        isblock0 = canexist0;
        isblock1 = canexist1;

        init_originalcoo();
        
        for(auto mode:modevec) lotation.lotate90(mode, isblock1, originalcoo);
        build_block();

        for(int i=0; i<4; i++){
            for(int j=0; j<4; j++){
                for(int k=0; k<4; k++){
                    int ni = (i+startmode2)%4;
                    int nj = (j+startmode2)%4;
                    int nk = (k+startmode2)%4;
                    vector<int> modevec2 = {ni, nj, nk};
                    init_originalcoo();
                    isblock0 = canexist0;
                    isblock1 = canexist1;
                    for(auto mode:modevec2) lotation.lotate90(mode, isblock1, originalcoo);
                    build_block();
                }
            }
        }        

        int bnum0 = dicube(nowblocknum0, silf0, silr0, canexist0, isblock0, blockset0);
        int bnum1 = dicube(nowblocknum1, silf1, silr1, canexist1, isblock1, blockset1);
        
        if(bnum0>bnum1){
            int diff = adddicube(bnum0-bnum1, silf1, silr1, canexist1, isblock1, blockset1);
            if(diff>0) adjust(diff, blockset0, silf0, silr0, isblock0);
        }
        else if(bnum1>bnum0) {
            int diff = adddicube(bnum1-bnum0, silf0, silr0, canexist0, isblock0, blockset0);
            if(diff>0) adjust(diff, blockset1, silf1, silr1, isblock1);
        }

        projectioin(silf0, silr0, isblock0, blockset0, canexist0);
        projectioin(silf1, silr1, isblock1, blockset1, canexist1);

        int monostart0 = max(blockset0.size(), blockset1.size());
        int monostart1 = monostart0;
        monocube(monostart0, silf0, silr0, canexist0, isblock0, blockset0);
        monocube(monostart1, silf1, silr1, canexist1, isblock1, blockset1);

        assignblocknum(ans0, blockset0);
        assignblocknum(ans1, blockset1);

        return;
    }

    void init_originalcoo(){
        for(int x=0; x<d; x++){
            for(int y=0; y<d; y++){
                for(int z=0; z<d; z++){
                    originalcoo[x][y][z] = {x,y,z};//回転しても元の座標がすぐわかるようにしておく
                }
            }
        }
        return;
    }

    void build_block(){
        for(int x=0; x<d; x++){
            for(int y=0; y<d; y++){
                for(int z=0; z<d; z++){
                    if(canexist0[x][y][z] && isblock1[x][y][z]){
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
        vector<Block> addblocks;

        common_bfs.bfs(isblock0, addblocks, blockset0.size());
        blockset0.insert(blockset0.end(), all(addblocks));

        putbackcoo(addblocks, originalcoo);
        blockset1.insert(blockset1.end(), all(addblocks));

        nowblocknum0 = blockset0.size();
        nowblocknum1 = blockset1.size();

        v3b tmp(init_false_v3b_d);
        isblock0 = tmp; isblock1 = tmp;
        //これまでに構成したブロックを投影する
        projectioin(silf0, silr0, isblock0, blockset0, canexist0);
        projectioin(silf1, silr1, isblock1, blockset1, canexist1);

        return;
    }

    void putbackcoo(vector<Block>& blockset, vector<vector<vector<vec3>>> lotatecoo){
        if(blockset.size()==0) return;
        for(int block=0; block<blockset.size(); block++){
            auto [ordernum, blocksize, cubevec] = blockset[block];
            vector<vec3> tmpcubevec;
            for(auto cube:cubevec){
                auto [cx, cy, cz] = cube;
                tmpcubevec.push_back(lotatecoo[cx][cy][cz]);
            }
            blockset[block].cooset.clear();
            blockset[block].cooset.shrink_to_fit();
            blockset[block].cooset = tmpcubevec;
        }
        return;
    }
    
    void projectioin(v2b& front, v2b& right, v3b& isblock, vector<Block> blockset, v3b& canexist){
        for(auto block:blockset){
            auto [ordernum, blocksize, cubevec] = block;
            for(auto cube:cubevec){
                auto [cx, cy, cz] = cube;
                isblock[cx][cy][cz] = true;
                canexist[cx][cy][cz] = false;
                front[cz][cx] = true;
                right[cz][cy] = true;
            }
        }
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
        return;
    }

    int dicube(int& nowblocknum, v2b& front, v2b& right, v3b canexist, v3b& isblock, vector<Block>& blockset){
        int preblocknum = nowblocknum;
        for(int x=0; x<d; x++){
            for(int y=0; y<d; y++){
                for(int z=0; z<d; z++){
                    if(canexist[x][y][z] && !isblock[x][y][z]){//モノキューブを置けるが置かれていない場所のみに置く
                        if(!front[z][x] || !right[z][y]){//片方のシルエットが空いていたらキューブを追加する
                            isblock[x][y][z] = true;
                            nowblocknum++;
                            blockset.push_back({nowblocknum, 1, {{x,y,z}}});
                            front[z][x] = true;
                            right[z][y] = true;

                            //キューブを連結して置けるかどうか
                            for(auto nv : dxdydz){
                                int nvx = x + nv.x;
                                int nvy = y + nv.y;
                                int nvz = z + nv.z;

                                if(nvx<0 || d<=nvx || nvy<0 || d<=nvy || nvz<0 || d<=nvz || !canexist[nvx][nvy][nvz] || isblock[nvx][nvy][nvz] || (front[nvz][nvx] && right[nvz][nvy])) continue;
                                // cout << nvx << " " << nvy << " " << nvz << " " << endl;

                                blockset[nowblocknum-1].cooset.push_back({nvx, nvy, nvz});
                                blockset[nowblocknum-1].siz++;
                                front[nvz][nvx] = true;
                                right[nvz][nvy] = true;
                                isblock[nvx][nvy][nvz] = true;

                                break;
                            }
                            if(blockset[nowblocknum-1].siz!=2){
                                blockset.pop_back();//dicubeを作れなかったら取り除く
                                nowblocknum--;
                                isblock[x][y][z] = false;
                                front[z][x] = false;
                                right[z][y] = false;
                            }
                        }
                        
                    }
                }
            }
        }
        return nowblocknum - preblocknum;
    }

    int adddicube(int diff, v2b& front, v2b& right, v3b canexist, v3b& isblock, vector<Block>& blockset){
        int addnum = 0;
        int presize = blockset.size();
        bool finished = false;
        for(int x=0; x<d; x++){
            for(int y=0; y<d; y++){
                for(int z=0; z<d; z++){
                    if(canexist[x][y][z] && !isblock[x][y][z]){
                        for(auto nv:dxdydz){
                            int nvx = x + nv.x;
                            int nvy = y + nv.y;
                            int nvz = z + nv.z;

                            if(nvx<0 || d<=nvx || nvy<0 || d<=nvy || nvz<0 || d<=nvz || !canexist[nvx][nvy][nvz] || isblock[nvx][nvy][nvz] ) continue;
                            
                            addnum++;
                            blockset.push_back({presize+addnum, 2, {{x,y,z},{nvx,nvy,nvz}}});
                            isblock[x][y][z] = true;
                            isblock[nvx][nvy][nvz] = true;
                            if(diff-addnum==0) finished = true;
                            break;
                        }
                    }
                    if(finished) break;
                }
                if(finished) break;
            }
            if(finished) break;
        }
        return diff-addnum;
    }

    void adjust(int diff, vector<Block>& blockset, v2b& front, v2b& right, v3b& isblock){
        //ブロックを削除してシルエットを調整する。ブロックを置いた座標はもともとシルエットが残っているところだったので、falseにしてOK
        for(int i=0; i<diff; i++){
            int vsize = blockset.size();
            vector<vec3> vset = blockset[vsize-1].cooset;
            blockset.pop_back();
            for(auto v:vset){
                auto [x, y, z] = v;
                isblock[x][y][z] = false;
                front[z][x] = false;
                right[z][y] = false;
            }
        }
    }

    //シルエットが埋まっていない位置に、モノキューブを置いていく。blocksetに追加していく。
    void monocube(int& nowblocknum, v2b& front, v2b& right, v3b canexist, v3b& isblock, vector<Block>& blockset){
        for(int z=0; z<d; z++){
            int unplaced = 0;
            for(int x=0; x<d; x++){
                for(int y=0; y<d; y++){
                    if(canexist[x][y][z] && !isblock[x][y][z]){//モノキューブを置けるが置かれていない場所のみに置く
                        if(!front[z][x] && !right[z][y]){//両方のシルエットが空いていたらキューブを追加する
                            isblock[x][y][z] = true;
                            nowblocknum++;
                            blockset.push_back({nowblocknum, 1, {{x,y,z}}});
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
                        nowblocknum++;
                        blockset.push_back({nowblocknum, 1, {{x,y,z}}});
                        front[z][x] = true;
                        right[z][y] = true;
                        unplaced--;
                        if(unplaced==0) break;
                    }
                }
                if(unplaced==0) break;
            }

            //テスト用。置ける場所すべてにモノキューブを置く。
            // for(int x=0; x<d; x++){
            //     for(int y=0; y<d; y++){
            //         if(canexist[x][y][z]){
            //             nowblocknum++;
            //             blockset.push_back({nowblocknum, 1, {{x,y,z}}});
            //         }
            //     }
            // }
        }
    }

    void assignblocknum(v3i& ans, vector<Block> blockset){
        for(auto block:blockset){
            auto [ordernum, blocksize, cubevec] = block;
            for(auto cube:cubevec){
                auto [cx, cy, cz] = cube;
                ans[cx][cy][cz] = ordernum;
            }
            if(blockinfo.find(ordernum)==blockinfo.end()) blockinfo[ordernum] = {1, blocksize};
            else blockinfo[ordernum].first++;
        }
    }

    void clear_v3all(v3b clearvec){
        for(int i=0; i<d; i++){
            for(int j=0; j<d; j++){
                clearvec[i][j].clear();
                clearvec[i][j].shrink_to_fit();
            }
            clearvec[i].clear();
            clearvec[i].shrink_to_fit();
        }
        clearvec.clear();
        clearvec.shrink_to_fit();
    }

};


int main(){

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

    long long minscore = 1e15;
    v3i ans0, ans1;
    int blocknum = 0;

    bool finished = false;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            for(int k=0; k<4; k++){

                for(int i2=0; i2<4; i2++){
                    vector<int> modevec = {i, j, k};

                    Solver solver(d, silhouette, modevec, i2);
                    solver.solve();

                    //スコア計算
                    double score = 0;
                    for(auto v:solver.blockinfo){
                        auto [num, iscommon] = v;
                        double tmpscore = INF;
                        if(iscommon.first==2) tmpscore = 1/(double)iscommon.second;
                        else tmpscore = (double)iscommon.second;
                        score += tmpscore;
                    }
                    long long finalscore = (long long)(score*1000000000);
                    // cout << finalscore << endl;

                    if(finalscore<minscore){
                        minscore = finalscore;
                        blocknum = max(solver.blockset0.back().num, solver.blockset1.back().num);
                        ans0 = solver.ans0;
                        ans1 = solver.ans1;
                    }
                }
            }
        }
    }

    //テスト用
    // vector<int> modevec = {2, 2, 2};
    // Solver solver(d, silhouette, modevec);
    // solver.solve();
    // blocknum = max(solver.blockset0.back().num, solver.blockset1.back().num);
    // ans0 = solver.ans0;
    // ans1 = solver.ans1;
    
    cout << blocknum << endl;

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
    cout << endl;

    // cout << minscore << endl;
    return 0;
}