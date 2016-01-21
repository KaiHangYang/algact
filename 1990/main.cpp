#include <cstdio>
#include <algorithm>
#include <cstring>

using namespace std;

#define MAX 20004

typedef struct {
    int t; // 声音的阈值
    int x; // 牛的横坐标
} cow;

int N; // 总大小
int mycount[MAX]; // 记录了听力比i小且坐标小等于i的牛数目
int mytotal[MAX]; // 记录了这些牛的总距离
cow cows[MAX];
unsigned long long allSum = 0;

int lowbit (int x) {
    return (-x)&x;
}
unsigned long long getsum(int i, int * treeArr) {
    unsigned long long sum = 0;
    while(i > 0) {
        sum += treeArr[i];
        i -= lowbit(i);
    }
    return sum;
}
void change(int i, int val, int * treeArr) {
    while(i <= MAX) {
        treeArr[i] += val;
        i += lowbit(i);
    }
}
// 用于排序的函数
bool my_cmp(cow x, cow y) {
    return x.t < y.t;
}

int main() {
    scanf("%d", &N);

    int i;
    for (i=1; i != N+1; ++i) {
        scanf("%d %d", &cows[i].t, &cows[i].x);
    }
    // 先对牛按照v坐标进行排序
    sort(cows+1, cows + N+ 1, my_cmp);
    unsigned long long sum = 0;
    memset(mycount, 0, sizeof(mycount));
    memset(mytotal, 0, sizeof(mytotal));

    for (i = 1; i != N+1; ++i) {
        // 其中i记录的就是听力小于v的节点的总数和。
        int x = cows[i].x;
        sum += cows[i].t * (getsum(x, mycount)*x - getsum(x, mytotal) + allSum - getsum(x, mytotal) - (i-getsum(x, mycount) - 1)*x);
        change(x, x, mytotal);
        change(x, 1, mycount);
        allSum += x;
    }
    printf("%lld", sum);
    return 0;
}