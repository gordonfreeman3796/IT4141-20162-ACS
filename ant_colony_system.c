#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#define MAX_IT 1250
#define NUM_ANTS 10
#define DECAY 0.1
#define C_HEUR 2.5
#define C_LOCAL_PHERO 0.1
#define C_GREED 0.9

typedef struct
{
    int *vector, cost;
} Solution;

typedef struct
{
    int city;
    double prob;
} Choice;

Solution search(double*, int, int, int, double, double, double, double);
int *random_permutation(int);
int cost(int*, double*, int);
int euc_2d(double*, int, int, int);
double *initialise_pheromone_matrix(int, double);
int *stepwise_const(double*, int, double*, double, double);
void calculate_choices(double*, int, int, int*, int, double*, double, double);
int greedy_select(Choice* , int);
int prob_select(Choice*, int);
void local_update_pheromone(double*, Solution, int, double, double);
void global_update_pheromone(double*, Solution, int, double);

Choice *choices = NULL;

main(int argc, char *argv[])
{
    char buff[255];
    int i, n, j;
    double *cities;
    srand(time(NULL));
    FILE *f = fopen(argv[1], "r");

    /* Coordinate */
    fgets(buff, 255, f);
    fgets(buff, 255, f);
    fgets(buff, 255, f);
    fscanf(f, "%s", buff);
    fscanf(f, "%d", &n);
    cities = (double*) malloc(n * 2 * sizeof(double));
    choices = (Choice*) malloc((n - 1) * sizeof(Choice));
    fgets(buff, 255, f);
    fgets(buff, 255, f);
    fgets(buff, 255, f);
    for (i = 0; i < n; i++)
    {
        fscanf(f, "%s", buff);
        fscanf(f, "%lf", cities + i);
        fscanf(f, "%lf", cities + i + n);
    }
    fclose(f);

    Solution best = search(cities, n, MAX_IT, NUM_ANTS, DECAY, C_HEUR, C_LOCAL_PHERO,C_GREED);
    printf("Done, Best Solution: c = %d, v = ", best.cost);
    for (i = 0; i < n; i++)
    {
        if (i == n - 1)
            printf("%d", best.vector[i]);
        else
            printf("%d, ", best.vector[i]);
    }
    printf("\n");
}

Solution search(double *cities, int num_cities, int max_it, int num_ants, double decay, double c_heur, double c_local_phero, double c_greed)
{
    Solution best, cand;
    double init_pheromone, *pheromone = NULL;
    int i, j, k;
    best.vector = random_permutation(num_cities);
    best.cost = cost(best.vector, cities, num_cities);
    init_pheromone = 1.0 / (num_cities * best.cost);
    pheromone = initialise_pheromone_matrix(num_cities, init_pheromone);
    for (i = 0; i < max_it; i++)
    {
        for (j = 0; j < num_ants; j++)
        {
            cand.vector = stepwise_const(cities, num_cities, pheromone, c_heur, c_greed);
            cand.cost = cost(cand.vector, cities, num_cities);
            if (cand.cost < best.cost)
            {
                for (k = 0; k < num_cities; k++)
                    best.vector[k] = cand.vector[k];
                best.cost = cand.cost;
            }
            local_update_pheromone(pheromone, cand, num_cities, c_local_phero, init_pheromone);
            free(cand.vector);
        }
        global_update_pheromone(pheromone, best, num_cities, decay);
        printf(" > iteration %d, best = %d\n", i + 1, best.cost);
    }
    return best;
}

int *random_permutation(int n)
{
    int *arr = (int*) malloc(n * sizeof(int)), i, j, temp;
    for (i = 0; i < n; i++)
        arr[i] = i + 1;
    for (i = n - 1; i > 0; i--)
    {
        j = rand() % i;
        temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
    return arr;
}

int cost(int *arr, double *cities, int n)
{
    int distance = 0, i, j;
    for (i = 0; i < n; i++)
    {
        j = (i == n - 1) ? 0 : i + 1;
        distance += euc_2d(cities, arr[i] - 1, arr[j] - 1, n);
    }
    return distance;
}

int euc_2d(double *cities, int i, int j, int n)
{
    return (int) round(sqrt(pow(*(cities + i) - *(cities + j), 2) + pow(*(cities + i + n) - *(cities + j + n), 2)));
}

double *initialise_pheromone_matrix(int n, double init_pher)
{
    double *pher_matrix = (double*) malloc(n * n * sizeof(double));
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            *(pher_matrix + n * i + j) = init_pher;
    return pher_matrix;
}

int *stepwise_const(double *cities, int num_cities, double *pheromone, double c_heur, double c_greed)
{
    int perm_size = 1, q, q0 = c_greed * RAND_MAX, choices_size, next_city;
    int *perm = (int*) malloc(perm_size * sizeof(int));
    
    perm[0] = rand() % num_cities + 1;
    while (perm_size < num_cities)
    {
        calculate_choices(cities, num_cities, perm[perm_size - 1], perm, perm_size, pheromone, c_heur, 1.0);
        q = rand();
        choices_size = num_cities - perm_size;
        next_city = (q <= q0) ? greedy_select(choices, choices_size) : prob_select(choices, choices_size);
        perm_size++;
        perm = (int*) realloc(perm, perm_size * sizeof(int));
        perm[perm_size - 1] = next_city;
    }
    return perm;
}

void calculate_choices(double *cities, int num_cities, int last_city, int *exclude, int exclude_size, double *pheromone, double c_heur, double c_hist)
{
    int i, j, k = 0, exclude_flag = 0;
    for (i = 1; i <= num_cities; i++)
    {
        for (j = 0; j < exclude_size; j++)
            if (exclude[j] == i)
            {
                exclude_flag = 1;
                break;
            }
        if (!exclude_flag)
        {
            choices[k].city = i;
            choices[k].prob = pow(*(pheromone + num_cities * (last_city - 1) + (i - 1)), c_hist) * (1.0 / pow(euc_2d(cities,  last_city - 1, i - 1 , num_cities), c_heur));
            k++;
        }
        exclude_flag = 0;
    }
}

int greedy_select(Choice *choices, int choices_size)
{
    Choice max = choices[0];
    int i;
    for (i = 1; i < choices_size; i++)
        if (max.prob < choices[i].prob)
            max = choices[i];
    return max.city;
}

int prob_select(Choice *choices, int choices_size)
{
    double sum = 0.0, v;
    int i;
    for (i = 0; i < choices_size; i++)
        sum += choices[i].prob;
    if (sum == 0.0)
        return choices[rand() % choices_size].city;
    v = (1.0 * rand()) / (1.0 * RAND_MAX);
    for (i = 0; i < choices_size; i++)
    {
        v -= choices[i].prob / sum;
        if (v <= 0.0)
            return choices[i].city;
    }
    return choices[choices_size - 1].city;
}

void local_update_pheromone(double *pheromone, Solution cand, int num_cities, double c_local_phero, double init_pheromone)
{
    int i, j, x, y;
    double value;
    for (i = 0; i < num_cities; i++)
    {
        j = (i == num_cities - 1) ? 0 : i + 1;
        x = cand.vector[i] - 1;
        y = cand.vector[j] - 1;
        value = (1.0 - c_local_phero) * *(pheromone + num_cities * x + y) + (c_local_phero * init_pheromone);
        *(pheromone + num_cities * x + y) = value;
        *(pheromone + num_cities * y + x) = value;
    }
}

void global_update_pheromone(double *pheromone, Solution best, int num_cities, double decay)
{
    int i, j, x, y;
    double value;
    for (i = 0; i < num_cities; i++)
    {
        j = (i == num_cities - 1) ? 0 : i + 1;
        x = best.vector[i] - 1;
        y = best.vector[j] - 1;
        value = (1.0 - decay) * *(pheromone + num_cities * x + y) + (decay * (1.0 / best.cost));
        *(pheromone + num_cities * x + y) = value;
        *(pheromone + num_cities * y + x) = value;
    }
}
