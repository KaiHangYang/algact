#include <iostream>
#include <cstring>
#include <cstdio>
using namespace std;

const int MAX = 100500;
const int nMAX = 105;
const int mMAX = 1005;

int strnum;
char str[nMAX][mMAX];
int source[MAX];
int sa[MAX], rk[MAX], height[MAX];
int wa[MAX], wb[MAX], wv[MAX], wd[MAX];
bool vis[nMAX];
int id[MAX];
int anslen, anspos[mMAX], ansnum;

int cmp(int* r, int a, int b, int l)
{
	return r[a] == r[b] && r[a + l] == r[b + l];
}

void suffix(int* r, int n, int m)
{
	int i, j;
	for (i = 0; i < n; ++i)
	{
		//height[i] = 0;
		rk[i] = 0;
	}
	int p, *x = wa, *y = wb, *t;
	for (i = 0; i < m; ++i) wd[i] = 0;
	for (i = 0; i < n; ++i) wd[x[i] = r[i]]++;
	for (i = 1; i < m; ++i) wd[i] += wd[i - 1];
	for (i = n - 1; i >= 0; --i) sa[--wd[x[i]]] = i;
	for (j = 1, p = 1; p < n; j <<= 1, m = p)
	{
		for (p = 0, i = n - j; i < n; ++i) y[p++] = i;
		for (i = 0; i < n; ++i) if (sa[i] >= j) y[p++] = sa[i] - j;
		for (i = 0; i < n; ++i) wv[i] = x[y[i]];
		for (i = 0; i < m; ++i) wd[i] = 0;
		for (i = 0; i < n; ++i) wd[wv[i]]++;
		for (i = 1; i < m; ++i) wd[i] += wd[i - 1];
		for (i = n - 1; i >= 0; --i) sa[--wd[wv[i]]] = y[i];
		for (t = x, x = y, y = t, p = 1, x[sa[0]] = 0, i = 1; i < n; ++i)
		{
			x[sa[i]] = cmp(y , sa[i - 1], sa[i], j) ? p - 1 : p++;
		}
	}
}

void calheight(int* r, int n)
{
	int i, j, k = 0;
	for (i = 1; i <= n; ++i) rk[sa[i]] = i;
	for (i = 0; i < n; height[rk[i++]] = k)
	{
		for (k ? k-- : 0, j = sa[rk[i] - 1]; r[i + k] == r[j + k]; ++k);
	}
}

bool solve(int beg, int end)
{
	int tot = 0;
	int t = strnum >> 1;
	for (int i = 0; i < strnum; ++i) vis[i] = false;
	for (int i = beg; i <= end; ++i)
	{
		if (!vis[id[sa[i]]])
		{
			vis[id[sa[i]]] = true;
			++tot;
		}
		if (tot > t) return true;
	}
	return false;
}

bool group(int len, int n)
{
	bool res = false;
	int beg, end;
	beg = end = 0;
	for (int i = 1; i < n; ++i)
	{
		if (height[i] >= len) ++end;
		else
		{
			if (solve(beg, end))
			{
				if (!res) ansnum = 0;
				res = true;
				anspos[ansnum++] = sa[beg];
			}
			beg = end = i;
		}
	}
	if (beg < end)
	{
		if (solve(beg, end))
		{
			if (!res) ansnum = 0;
			res = true;
			anspos[ansnum++] = sa[beg];
		}
	}
	return res;
}

int main()
{
	while (scanf("%d", &strnum) && strnum != 0)
	{
		for (int i = 0; i < strnum; ++i) scanf("%s", str[i]);
		int n = 0, low = 1, high = 0, mid;
		for (int i = 0; i < strnum; ++i)
		{
			int j;
			for (j = 0; str[i][j] != 0; ++j)
			{
				id[n] = i;
				source[n++] = str[i][j] - 'a' + 100;
			}
			if (j > high) high = j;
			id[n] = i;
			source[n++] = i;
		}
		suffix(source, n, 126);
		calheight(source, n - 1);
		anslen = 0;
		while (low <= high)
		{
			mid = (low + high) >> 1;
			if (group(mid, n))
			{
				anslen = mid;
				low = mid + 1;
			}
			else high = mid - 1;
		}
		if (anslen == 0) printf("?\n");
		else
		{
			for (int i = 0; i < ansnum; ++i)
			{
				for (int j = 0; j < anslen; ++j)
				{
					printf("%c", source[anspos[i] + j] - 100 + 'a');
				}
				printf("\n");
			}
		}
		printf("\n");
	}
	return 0;
}