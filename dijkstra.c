#include <stdio.h>
#include <limits.h>
#include <malloc.h>
#include <stack>

using namespace std;

int find_min(int *t, int *arr, int n)
{
    int i, min, j;
    for (i = 1; i < n; i++)
        if (t[i])
        {
            min = arr[i];
            j = i;
        }       

    for (i = j + 1; i < n; i++)
        if (t[i] && arr[i] < min)
            min = arr[i];

    return min;
}

int is_empty(int *arr, int n)
{
    int i;
    for (i = 0; i < n; i++)
        if (arr[i])
            return 0;
    return 1;
}

void print_result(int *arr, int n, int src, int dst)
{
    int i = arr[dst];
    stack<int> path;
    path.push(dst);
    while (i != src)
    {
        path.push(i);
        i = arr[i];
    }
    path.push(src);
    while (!path.empty())
    {
        printf("%d ", path.top() + 1);
        path.pop();
    }
}

int main()
{
    int n, m, *w, *pre, src, dst, i, j, v1, v2, *d, *t, u;
    FILE *f = fopen("input.txt", "r");
    fscanf(f, "%d", &n);
    fscanf(f, "%d", &m);
    /*scanf("%d", &n);
    scanf("%d", &m);*/
    w = (int*) malloc(n * n * sizeof(int));
    pre = (int*) malloc(n * sizeof(int));
    d = (int*) malloc(n * sizeof(int));
    t = (int*) malloc(n * sizeof(int));
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            *(w + i * n + j) = INT_MAX;
    for (i = 0; i < m; i++)
    {
        fscanf(f, "%d", &v1);
        fscanf(f, "%d", &v2);
        fscanf(f, "%d", w + (v1 - 1) * n + (v2 - 1));
        /*scanf("%d", &v1);
        scanf("%d", &v2);
        scanf("%d", w + (v1 - 1) * n + (v2 - 1));*/
    }
    fscanf(f, "%d", &src);
    fscanf(f, "%d", &dst);
    /*scanf("%d", &src);
    scanf("%d", &dst);*/
    for (i = 0; i < n; i++)
    {
        *(pre + i) = src;
        *(d + i) = *(w + n *(src - 1) + i);
        if (i != src - 1)
            t[i] = 1;
        else
            t[i] = 0;
    }

    /*for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
            printf("%10d ", *(w + n * i + j));
        printf("\n");
    }*/
    for (i = 0; i < n; i++)
        printf("%d ", *(pre + i));
    fclose(f);
    
    while (!is_empty(t, n))
    {
        u = find_min(t, d, n);
        t[u] = 0;
        for (i = 0; i < n; i++)
            if (t[i])
                if (*(d + i) > d[u] + *(w + n * u + i))
                {
                    *(d + i) = d[u] + *(w + n * u + i);
                    *(pre + i) = u;
                }
    }
    printf("%d\n", d[dst]);
    print_result(pre, n, src, dst);
}
