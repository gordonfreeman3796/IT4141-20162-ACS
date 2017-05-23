#include <stdio.h>
#include <limits.h>
#include <malloc.h>

int find_min(int *arr, int n)
{
    int i, min;
    min = arr[0]
    for (i = 1; i < n; i++)
    {
        if (
    }
}

int main()
{
    int n, m, *w, *pre, src, dst, i, j, v1, v2, *d, *t, u;
    /*FILE *f = fopen("input.txt", "r");
    fscanf(f, "%d", &n);
    fscanf(f, "%d", &m);*/
    scanf("%d", &n);
    scanf("%d", &m);
    w = (int*) malloc(n * n * sizeof(int));
    pre = (int*) malloc(n * sizeof(int));
    d = (int*) malloc(n * sizeof(int));
    t = (int*) malloc(n * sizeof(int));
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            *(w + i * n + j) = INT_MAX;
    for (i = 0; i < m; i++)
    {
        /*fscanf(f, "%d", &v1);
        fscanf(f, "%d", &v2);
        fscanf(f, "%d", w + (v1 - 1) * n + (v2 - 1));*/
        scanf("%d", &v1);
        scanf("%d", &v2);
        scanf("%d", w + (v1 - 1) * n + (v2 - 1));
    }
    /*fscanf(f, "%d", &s);
    fscanf(f, "%d", &d);*/
    scanf("%d", &src);
    scanf("%d", &dst);
    for (i = 0; i < n; i++)
    {
        *(pre + i) = src;
        *(d + i) = w[src - 1][i];
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
    }
    for (i = 0; i < n; i++)
        printf("%d ", *(pre + i));
    fclose(f);*/
    
    while (!is_empty(t))
    {
        u = find_min(d);
        t[u] = 0;
        for (i = 0; i < n; i++)
            if (t[i])
                if (d[i] > d[u] + w[u][i])
                {
                    d[i] = d[u] + w[u][i];
                    p[i] = u;
                }
    }
}
