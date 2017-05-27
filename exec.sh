gcc -c ant_colony_system.c
gcc -o ant_colony_system ant_colony_system.o -lm
./ant_colony_system $1
