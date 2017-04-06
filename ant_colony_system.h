#ifndef __ANT_COLONY_SYSTEM_H__
#define __ANT_COLONY_SYSTEM_H__

#define MAX_IT 100
#define NUM_ANTS 10
#define DECAY 0.1
#define C_HEUR 2.5
#define C_LOCAL_PHERO 0.1
#define C_GREED 0.9
#define CITY_SIZE 52

typedef struct
{
    int vector[52], cost;
} Solution;

typedef struct
{
    int city, distance;
    float history, heuristice, prob;
} Probability;

int search(int**, int, int, float, float, float, float);
int euc_2d(int*, int*);
int cost(int*, int**);
void random_permutation(int*, int);
void initialise_pheromone_matrix(int**, int, float);
void calculate_choices(Probability*, int**, int, int*, int**, float, float);
int prob_select(Probability*, int);
void greedy_select(Probability*, int);
void stepwise_const(int*, int**, int**, float, float);
void global_update_pheromone(float**, Solution, float);
void local_update_pheromone(float**, Solution, float, float);

#endif
