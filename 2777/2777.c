#include <stdio.h>
#include <string.h>


#define MAX 100002
typedef char bool;
#define true 1
#define false 0
struct Node {
	int color;
	int left, right; // 表示的范围
	struct Node *nleft, *nright;// 指向的字节点位置
    bool stop; // 表示这个节点是否已经能够表示整段区间的颜色
} tree[MAX*3];
// 这里因为只有30种颜色，使用整数的每一位表示一个颜色可以很容易的计算颜色数目
int L, T=0, O;
char ch;
int start, end, color;

int num = 0;
void tree_build(struct Node * node, int left, int right);
void draw(struct Node * node, int left, int right, int color);
int query(struct Node * node, int left, int right);
int getnum(int num);

int main() {
    //freopen("tmp", "r", stdin);
	scanf("%d %d %d", &L, &T, &O);
    getchar();

    //build the line tree
    tree_build(tree, 1, L);
    int i;
	for (i=0; i != O; ++i) {
		scanf("%c", &ch);
		if (ch == 'C') {
			// 添加颜色
			scanf("%d %d %d", &start, &end, &color);
            getchar();
            if (start > end) {
                int tmp = start;
                start = end;
                end = tmp;
            }

            draw(tree, start, end, (1 << (color-1)));
			
		}
		else if (ch == 'P') {
			scanf("%d %d", &start, &end);
            getchar();
            if (start > end) {
                int tmp = start;
                start = end;
                end = tmp;
            }

            printf("%d\n", getnum(query(tree, start, end)));
		}
		
	}

	return 0;
}


void tree_build(struct Node * node, int left, int right) {
    // cause i can paint one point, the tree must contain points
	node->left = left;
	node->right = right;
	node->color = 1; 
	node->stop = true;
	if (left == right) {
		return;
	}

	int tmp = (left+right)/2;
    ++num;
    node->nleft = tree+num;
    ++num;
    node->nright = tree+num;

    tree_build(node->nleft, left, tmp);
    tree_build(node->nright, tmp+1, right);
}


void draw(struct Node * node, int left, int right, int color) {
    if (node->left == left && node->right == right) {
        // 恰好是这个节点
        node->stop = true;
        node->color = color;
        return;
    }
    
    if (node->stop) {
        node->stop = false;
        node->nleft->color = node->color;
        node->nleft->stop = true;
        node->nright->color = node->color;
        node->nright->stop = true;
    }

    int mid = (node->left+node->right)/2;
    
    if (right <= mid) {
        draw(node->nleft, left, right, color);
    }
    else if (left > mid) {
        draw(node->nright, left, right, color);
    }
    else {
        draw(node->nleft, left, mid, color);
        draw(node->nright, mid+1, right, color);
    }
    node->color = node->nleft->color | node->nright->color;
}

int query(struct Node * node, int left, int right) {
    if (node->stop) {
        return node->color;
    }
    if (node->left == left, node->right == right) {
        return node->color;
    }

    int mid = (node->left + node->right)/2;

    if (right <= mid) {
        return query(node->nleft, left, right);
    }
    else if (left > mid) {
        return query(node->nright, left, right);
    }
    else {
        return query(node->nleft, left, mid) | query(node->nright, mid+1, right);
    }
}

int getnum(int num) {
    // 计算结果表示的颜色数目
    int mask = 1;
    int sum = 0;
    int i;
    for(i = 0; i != T; ++i, mask <<= 1) {
        if (mask & num) {
            ++sum;
        }
    }

    return sum;
}
