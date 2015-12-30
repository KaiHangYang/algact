#include <cstdio>
#include <vector>
#include <cstring>

#define MAX 100002

using namespace std;

int N, M; // 边总数
char query;
int qpos;
int x, y; // 边的编号
int num = 0;


// 实际上treeArr 仅仅是用来保存个数的,没有什么形式上的相关性
int treeArr[MAX];
//int visited[MAX];

int myfork[MAX]; // 记录每个枝桠的苹果
// 仅仅是用来管理树状数组的选择范围
int mystart[MAX]; // 储存节点的序号
int myend[MAX]; // 储存节点管辖的范围

// 临界表的实现用vector的vector竟然比数组还快````
vector<vector<int>>edges(MAX); // 树关系的邻接表

int lowbit(int n);
void change(int pos, int num);
int getsum(int pos);
// 对树进行DFS,将树转化为树状数组
void dfs(int pos);

int main() {

    memset(treeArr, 0, sizeof(treeArr));
    memset(myfork, 0, sizeof(myfork));
    memset(mystart, 0, sizeof(mystart));
    memset(myend, 0, sizeof(myend));
//    memset(visited, 0, sizeof(visited));

    scanf("%d", &N);
    // 树结构,N节点 N-1条边
    for (int i = 1; i < N; ++i) {
        scanf("%d %d", &x, &y);
        edges[x].push_back(y); // 构建邻接表
    }
    dfs(1); // 从一开始遍历
    // 初始化树状数组
    for (int i = 1; i <= N; ++i) {
        myfork[i] = 1;
        change(i, 1); // 更新数组的值
    }

    scanf("%d%*c", &M); // 将换行符丢弃

    for (int i = 0; i < M; ++i) {
        scanf("%c %d%*c", &query, &qpos);

        if (query == 'Q') {
            printf("%d\n", getsum(myend[qpos]) - getsum(mystart[qpos] - 1)); // 包含左边的数
        }
        else {
            if (myfork[qpos]) {
                change(mystart[qpos], -1);
            }
            else {
                change(mystart[qpos], 1);
            }

            myfork[qpos] = !myfork[qpos];
        }
    }
    return 0;
}

int lowbit(int n) {
    return (n & (-n)); // 获取最低位
}
void dfs(int pos) {
//    if (visited[pos]) {
//        return;
//    }
//    visited[pos] = 1;
    mystart[pos] = ++num; // 因为是从0开始的

    for (int i=0; i != edges[pos].size(); ++i) {
        dfs(edges[pos].at(i));
    }
    // 遍历了所有的子节点之后 得到pos的管辖范围
    myend[pos] = num;
}
void change(int pos, int n) {
    while(pos <= N) {
        treeArr[pos] += n;
        pos += lowbit(pos);
    }
}
int getsum(int pos) {
    int sum = 0;
    while(pos >= 1) {
        sum += treeArr[pos];
        pos -= lowbit(pos);
    }
    return sum;
}