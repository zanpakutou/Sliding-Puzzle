#include <bits/stdc++.h>
#include "solution.h"
#include "board.h"

using namespace std;

int per[] = {0, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880};
int cdf[362880 + 10], dist[362880 + 10], previous[362880 + 10];
vector< int > valid;

void initlistpermutation()
{
    int cnt = 0;
    cdf[0] = 123456789;
    vector< int > v;
    for(int i = 1; i <= 9; i++) v.push_back(i);
    while(next_permutation(v.begin(), v.end())){
        int n = 0;
        cnt++;
        for(auto i : v) n = n * 10 + i;
        cdf[cnt] = n;
    }
}
int pertoRank(int p)
{
    int res = 0;
    vector< int > v;
    while(p) v.push_back(p % 10), p/=10;
    for(int i = 0; i < 9; i++){
        int ord = 0;
        for(int j = 0; j < i; j++)
            if (v[j] < v[i]) ord++;
        res+= ord * per[i];
    }
    return res;
}
int ranktoPer(int n)
{
    return cdf[n];
}
void Bfs()
{

    initlistpermutation();

    memset(dist, -1, sizeof dist);
    dist[0] = 0;
    queue< int > Q;
    Q.push(0);
    while(!Q.empty()){
        int recent = Q.front();
        int P = cdf[recent];
        valid.push_back(P);
        Q.pop();
        int new_P;
        new_P = pertoRank(board(P).moveUp().getID());
        if (dist[new_P] == -1){
            Q.push(new_P);
            dist[new_P] = dist[recent] + 1;
            previous[new_P] = 1;
        }
        new_P = pertoRank(board(P).moveDown().getID());
        if (dist[new_P] == -1){
            Q.push(new_P);
            dist[new_P] = dist[recent] + 1;
            previous[new_P] = 2;
        }
        new_P = pertoRank(board(P).moveLeft().getID());

        if (dist[new_P] == -1){
            Q.push(new_P);
            dist[new_P] = dist[recent] + 1;
            previous[new_P] = 3;
        }
        new_P = pertoRank(board(P).moveRigh().getID());
        if (dist[new_P] == -1){
            Q.push(new_P);
            dist[new_P] = dist[recent] + 1;
            previous[new_P] = 4;
        }
    }
}
int trace(int p)
{
    return previous[pertoRank(p)];
}
int randseg(int L, int R)
{
    return 1LL * rand() * rand() % (R - L + 1) + L;
}
int rand(int level)
{
    if (level == 1)
        return valid[randseg(0, 1000)];
    if (level == 2)
        return valid[randseg(1000, 10000)];
    if (level == 3)
        return valid[randseg(10000, 70000)];
    if (level == 4)
        return valid[randseg(70000, 140000)];
    if (level == 5)
        return valid[randseg(140000, 181439)];
}
