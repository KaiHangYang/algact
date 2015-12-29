#include <stdio.h>

#define MAX 50001
// 因为使用了递归来简化操作,所以说使用C++的话会很费时间,所以改用C
int find(int x);
void merge(int x, int y, int d);
int arr[MAX];
int myrank[MAX];
int N;
// 之前没有想到带全的并查集```` XYZ之间的关系是可以通过两个条件推导的
int main() {
    int K, D, X, Y;
    int i, result;

    result = 0;

    scanf("%d %d", &N, &K);

    // 初始化
    for (i=0; i != N+1; ++i) {
        arr[i] = i;
        myrank[i] = 0;
    }

    for (i=0; i != K; ++i) {
        scanf("%d %d %d", &D, &X, &Y);
        // 如果拥有同一个祖先的话,就可以判断是否是有问题,如果没有的话,需要开新组,也就是将点加进去.
        if (X > N || X <= 0 || Y > N || X <= 0 || ((X == Y) && (D == 2))) {
            // 简单的条件判断 这种条件下的语句绝对错误
            ++result;
        }
        else {
            int tmpx = find(X);
            int tmpy = find(Y);

            if (tmpx != tmpy) {
                // 说明不在同一个 "树"内
                merge(X, Y, D);
            }
            else {
                if (D == 1) {
                    if (myrank[X] == myrank[Y]) {
                        // 仅当 X Y 和根节点关系相同时成立
                        merge(X, Y, D); // 将新条件加进去
                    }
                    else {
                        ++result;
                    }
                }
                else if (D == 2) {
                    if ((myrank[X] == 0 && myrank[Y] == 2) || (myrank[X] == 2 && myrank[Y] == 1) || (myrank[X] == 1 && myrank[Y] == 0)) {
                        // 三种关系能够导致 X吃Y
                        merge(X, Y, D);
                    }
                    else {
                        ++result;
                    }
                }
            }
        }
    }
    printf("%d\n", result);
    return 0;
}

int find(int x) {
    // 原来使用的是循环,但是考虑到要修改过程中的myrank,需要使用"首"递归
    if (x == arr[x]) {
        return x;
    }
    else {
        // 这里是压缩过程 边压缩边寻找根节点
        int tmp = arr[x];
        arr[x] = find(tmp);
        // 这里修改的时候 tmp的rank已经是相当于根节点的了,所以可以直接计算
        // 这部分 后序遍历 大大减少了计算rank的计算量
        myrank[x] = (myrank[x]+myrank[tmp])%3;
        return arr[x];
    }

}
void merge(int x, int y, int d) {
    // 合并过程还要进行对myrank的更新
    int tmpx = find(x);
    int tmpy = find(y);

    // 相等的话就不管了
    if (tmpx != tmpy) {
        arr[tmpx] = tmpy; // 将tmpy这个树加到tmpx这个树上

        myrank[tmpx] = (((d-1)+myrank[y]) + (3 - myrank[x])%3)%3; // X -> tmpY + tmpX -> X
    }
}