#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>

using namespace std;

#define MAX 602
int n, p1, p2;
int x, y;
char str[5];

int dp[MAX][MAX/2];
int pre[MAX][MAX/2];// 前驱矩阵,用于输出中间结果
vector<int> sarr[MAX][2];

int arr[MAX];
char myrank[MAX]; // 记录 元素相对与根节点的关系 0 同类 1 不同类
int arrSum[MAX][2]; // 记录每个集合中两类的个数 其中arrNum[~][0]表示的是根节点代表的那类的个数
int roots[MAX];
int * isExist;

int find(int x);
void merge(int x, int y, char isYes);

// 如果为 yes 同属一个集合,用于构建并查集
// 如果为 no 不属于同一集合,用于表示两集合关系.
// 并查集用来表示相关的关系 并不是相同的放在同一组

int main() {
    scanf("%d %d %d", &n, &p1, &p2);
    // 首先构造相应的并查集和
    while (!(n == 0 && p1 == 0 && p2 == 0)) {
        memset(dp, 0, sizeof(dp));
        memset(pre, 0, sizeof(pre));

        isExist = new int[p1+p2];
        memset(isExist, 0, sizeof(*isExist));

        for (int i=0; i != p1+p2+1; ++i) {
            arr[i] = i;
            myrank[i] = 0;
            arrSum[i][0] = 1;
            arrSum[i][1] = 0;
            roots[i] = 0;
        }

        for (int i=0; i != n; ++i) {
            scanf("%d %d %s", &x, &y, str);

            char rel = 0;

            if (str[0] == 'n') {
                rel = 1;
            }

            int tmpx = find(x);
            int tmpy = find(y);

            // 记录出现过的点
            isExist[x] = 1;
            isExist[y] = 1;

            if (tmpx != tmpy) {
                merge(x, y , rel);
            }

        }
        // 构造完毕 可以开始进行统计 背包问题
        int sum = 0;
        vector<int> rootNum;

        // 需要对节点重新进行压缩.
        int total = 0;
        for (int i=1; i != p1+p2+1; ++i) {
                // 首先这个点出现过
            if (isExist[i]) {
                total++;
                int tmp = find(i);
                if (!roots[tmp]) {
                    rootNum.push_back(tmp);
                    ++sum;
                    roots[tmp] = 1;
                }
            }
        }
        // 接下来就是一个0 1背包问题 使用动态规划来解
        dp[0][0] = 1;
        for (int i=1; i != sum+1; ++i) {
            // 这里的i没有什么特殊的意义
            int x = rootNum.at(i-1);

            for (int j=p1; j >= 0; --j) {
                if (j-arrSum[x][0] >= 0 && dp[i-1][j-arrSum[x][0]]) {
                    dp[i][j] += dp[i-1][j-arrSum[x][0]];
                    pre[i][j] = j-arrSum[x][0]; // 记录上一条路径
                }

                if (j-arrSum[x][1] >= 0 && dp[i-1][j-arrSum[x][1]]) {
                    dp[i][j] += dp[i-1][j-arrSum[x][1]];
                    pre[i][j] = j-arrSum[x][1];
                }
            }
        }
        if (dp[sum][p1] != 1 || total != p1 + p2) {
            // 不能满足条件
            printf("no\n");
        }
        else {
            // 满足条件了
            for (int i=1; i != p1+p2+1; ++i) {
                if (roots[arr[i]]) {
                    sarr[arr[i]][myrank[i]].push_back(i);
                }
            }
            vector<int> result;

            int tmp1 = p1;

            for (int i=sum; i != 0; --i) {
                int tmp2 = tmp1 - pre[i][tmp1];
                int tmpx = rootNum.at(i-1);

                if (tmp2 == arrSum[tmpx][0]) {
                    for (int j=0; j != arrSum[tmpx][0]; ++j) {
                        result.push_back(sarr[tmpx][0].at(j));
                    }
                    sarr[tmpx][0].clear();
                    sarr[tmpx][1].clear();
                }
                else if (tmp2 == arrSum[tmpx][1]) {
                    for (int j=0; j != arrSum[tmpx][1]; ++j) {
                        result.push_back(sarr[tmpx][1].at(j));
                    }
                    sarr[tmpx][0].clear();
                    sarr[tmpx][1].clear();
                }
                tmp1 = pre[i][tmp1];
            }
            sort(result.begin(), result.end());
            for (int i=0; i != result.size(); ++i) {
                printf("%d\n", result.at(i));
            }
            printf("end\n");
        }
        delete(isExist);
        scanf("%d %d %d", &n, &p1, &p2);
    }

}

int find(int x) {
    if (arr[x] == x) {
        return x;
    }
    else {
        int tmp = arr[x];
        arr[x] = find(tmp);
        myrank[x] = (myrank[tmp] + myrank[x])%(char)(2);
        return arr[x];
    }
}
void merge(int x, int y, char isYes) {
    int tmpx = find(x);
    int tmpy = find(y);

    if (tmpx != tmpy) {
        // 两者不在同一组
        arr[tmpx] = tmpy;
        myrank[tmpx] = ((isYes + myrank[y])%(char)2 + myrank[x])%(char)2; // 关系是相互的
        // 重新计算数目 因为只有合并过程,所以说仅仅加法就够了
        if (myrank[tmpx] == 0) {
            arrSum[tmpy][0] += arrSum[tmpx][0];
            arrSum[tmpy][1] += arrSum[tmpx][1];
        }
        else {
            arrSum[tmpy][0] += arrSum[tmpx][1];
            arrSum[tmpy][1] += arrSum[tmpx][0];
        }
    }

}