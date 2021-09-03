/// Simulates a forest fire
/// @author Ethan Iannicelli
///

/*
    Revision History:
    Revision 1 (2:23:2021:16:00):
        started an outline, mostly the busywork and starting the porccess of intaking the flags, no real logic yet, also added global variables and their defaults.
    Revision 2 (2:28:2021:14:50):
        completed the cmd line processing, got some of the -pN logic out of the way, still need the start() and spread() logic. also need to implement the default display.
    Revision 3 (3:2:2021:18:02):
        completed the start function to initiate the forest
    Revision 4 (3:3:2021:22:55):
        completed neighbors counting function
    Revision 5 (3:4:2021:14:13):
        completed the spread function and fixed the neighbors counting function to match project desc
    Revision 6 (3:7:2021:18:11):
        completed display and fixed numerous errors, works as intended except for: display cuts off row 0 and col 0, prob_catch is not used correctly, and neighbors influence is not working as intended.
    Revision 7 (3:8:2021:10:28):
        fixed display output.
    Revision 8 (3:8:2021:22:52):
        fixed trees not catching fire correctly and possible neighbor error
    Revision 9 (3:9:2021:13:05):
        added the lightning and dampness options and the logic
*/

#ifndef DISPLAY_H

#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "display.h"
#include <getopt.h>
#include <unistd.h>

#define DEFAULT_BURN 10     ///used if not override on command line
#define DEFAULT_PROB_CATCH 30       ///used if not override on command line
#define DEFAULT_DENSITY 50      ///used if not override on command line
#define DEFAULT_PROP_NEIGHBOR 25     ///used if not override on command line
#define DEFAULT_PRINT_COUNT 0       ///default is 0 which is false
#define DEFAULT_SIZE 10     ///used if not override on command line
#define DEFAULT_LIGHTNING 0     ///used if not ovveride on command line
#define DEFAULT_DAMPNESS 0      ///used if not override on command line

#endif
double init_burn = DEFAULT_BURN/100.0;
double prob_catch = DEFAULT_PROB_CATCH/100.0;
double density = DEFAULT_DENSITY/100.0;
double neighbors = DEFAULT_PROP_NEIGHBOR/100.0;
int print_count = DEFAULT_PRINT_COUNT; 
int size = DEFAULT_SIZE;
double lightning = DEFAULT_LIGHTNING/100.0;
double dampness = DEFAULT_DAMPNESS/100.0;

/// counts the neighbors of a tree to see how many are burning
/// @param row: row of a tree
/// @param col: column of a row
/// @return the number of burning neighbors
double count_neighbors(int row, int col, char forest[size][size]) {
    ///total neighbors
    double count_of_neighbors = 0;
    ///burning neighbors
    double burned_neighbors = 0;

    ///number of burned trees
    if ((row > 0 && col > 0) && forest[row-1][col-1] == '*'){
        burned_neighbors++;
        count_of_neighbors++;
    }
    if ((row > 0) && forest[row-1][col] == '*'){
        burned_neighbors++;
        count_of_neighbors++;
    }
    if ((row > 0 && col < size-1) && forest[row-1][col+1] == '*'){
        burned_neighbors++;
        count_of_neighbors++;
    }
    if ((col > 0) && forest[row][col-1] == '*'){
        burned_neighbors++;
        count_of_neighbors++;
    }
    if ((col < size-1) && forest[row][col+1] == '*'){
        burned_neighbors++;
        count_of_neighbors++;
    }
    if ((row < size-1 && col > 0) && forest[row+1][col-1] == '*'){
        burned_neighbors++;
        count_of_neighbors++;
    }
    if ((row < size-1) && forest[row+1][col] == '*'){
        burned_neighbors++;
        count_of_neighbors++;
    }
    if ((row < size-1 && col < size-1) && forest[row+1][col+1] == '*'){
        burned_neighbors++;
        count_of_neighbors++;
    }
    ///number of unburned trees;
    if ((row > 0 && col > 0) && forest[row-1][col-1] == 'Y')
        count_of_neighbors++;
    if ((row > 0) && forest[row-1][col] == 'Y')
        count_of_neighbors++;
    if ((row > 0 && col < size-1) && forest[row-1][col+1] == 'Y')
        count_of_neighbors++;
    if ((col > 0) && forest[row][col-1] == 'Y')
        count_of_neighbors++;
    if ((col < size-1) && forest[row][col+1] == 'Y')
        count_of_neighbors++;
    if ((row < size-1 && col > 0) && forest[row+1][col-1] == 'Y')
        count_of_neighbors++;
    if ((row < size-1) && forest[row+1][col] == 'Y')
        count_of_neighbors++;
    if ((row < size-1 && col < size-1) && forest[row+1][col+1] == 'Y')
        count_of_neighbors++;
    
    if (count_of_neighbors == 0)
        return 0;

    return burned_neighbors/count_of_neighbors;
}

/// checks to see if the fire kills a tree or spreads to other trees if possible
/// @post: fire has progressed
void spread(char forest[size][size], int burned_forest[size][size]) {
    ///new cycle of the fire
    char new_forest[size][size];
    for (int r=0; r<size; r++) {
        for (int c=0; c<size; c++) {
            ///checks if tree and if the neighbor count is high enough to start burning
            if (forest[r][c] == 'Y') {
                double nearby = count_neighbors(r, c, forest);
                int chance = (int)((rand() / ((double) RAND_MAX))+(prob_catch));
                if (nearby >= neighbors && chance == 1) {
                    new_forest[r][c] = '*';
                    burned_forest[r][c]++;
                } else {
                    new_forest[r][c] = 'Y';
                }
            ///checks if burning and if has been burning for 3 cycles
            } else if (forest[r][c] == '*') {
                if (burned_forest[r][c] == 3) {
                    new_forest[r][c] = '.';
                } else {
                    new_forest[r][c] = '*';
                    burned_forest[r][c]++;
                }
            ///checks if burned out
            } else if (forest[r][c] == '.') {
                new_forest[r][c] = '.';
            ///checks if empty
            } else {
                new_forest[r][c] = ' ';
            }
        }
    }

    ///simulates a lightning strike
    int strike = (int)((rand() / ((double)RAND_MAX))+lightning);
    if (strike == 1) {
        int r = rand() % size;
        int c = rand() % size;
        if (new_forest[r][c] == 'Y'){
            new_forest[r][c] = '*';
            burned_forest[r][c]++;
            printf("LIGHTNING STRIKES\n");
        }
    }

    ///copies new_forest to forest
    for (int r=0; r<size; r++)
        for (int c=0; c<size; c++)
            forest[r][c] = new_forest[r][c];
}

/// creates and prints the first cycle of the fire
/// @param size: size of the forest
/// @param d: % of cells that are trees
/// @param b: % of trees that start as burning
/// @post forest has been created
/// @return initiated forest
void start (char forest[size][size], int burned_forest[size][size]) {
    int d = size*size*density;
    int b = d*(init_burn);
    int count = 0;
    srandom(41);

    ///creates forest with a number of trees d in it
    for (int i=0; i<d; i++){ 
        int row = random();
        row %= size;
        int col = random();
        col %= size;
        if (forest[row][col] == 'Y')
            i--;
        forest[row][col] = 'Y';
    }

    ///starts burning a number b trees
    while (count < b) {
        for (int r=0; r<size; r++) {
            for (int c=0; c<size; c++) {
                if (forest[r][c] == 'Y') {
                    if ((int)((rand() / ((double) RAND_MAX))+(init_burn)) == 1) {
                        forest[r][c] = '*';
                        count++;
                    }
                }
            }
        }
    }

    ///sets empty cells to ' '
    for (int r=0; r<size; r++){
        for (int c=0; c<size; c++){
            if (forest[r][c] != 'Y' && forest[r][c] != '*')
                forest[r][c] = ' ';
        }
    }

    ///sets the burned_forest values to 0
    for (int r=0; r<size; r++)
        for (int c=0; c<size; c++)
            burned_forest[r][c] = 0;
}


/// processes command line prompts as flags
/// @param argc: number of arguments
/// @param argv: command prompts
/// @return: 0 for EXIT_SUCCESS, -1 if failed
int main(int argc, char * argv[] ){
    int opt;
    int tmp = 0;
    while ((opt = getopt(argc, argv, "Hb:c:d:n:p:s:l:D:")) != -1) {
        switch (opt) {
            case 'H':
                fprintf( stderr, "\nusage: wildfire [options] \nBy default, the simulation runs in overlay mode. \nThe -pN options makes the simulation run in print mode for up to N states. \n\nSimulation Configuration Options: \n -H || View simulation options and quit. \n -bN || proportion of trees that are already burning. \n -cN || probability that a tree will catch fire. 0 < N < 101. \n -dN || density: the proportion of trees in the grid. 0< N < 101. \n -nN || proportion of neighbors that influece a tree catching fire. -1 < N < 101. \n -pN || number of states to print before quitting. -1 < N < ... \n -sN || simulation grid size. 4 < N < 41. \n \n");
                return 0;
            case 'b': 
                tmp = (int)strtol(optarg, NULL, 10);
                    if (tmp <= 0 || tmp > 100) {
                    fprintf(stderr, "The -b option requires a number from 1 to 100\n");
                    return -1;
                }
                init_burn = tmp/100.0;
                break;
            case 'c':
                tmp = (int)strtol(optarg, NULL, 10);
                if (tmp <= 0 || tmp > 100) {
                    fprintf(stderr, "The -c option requires a number from 1 to 100\n");
                    return -1;
                }
                prob_catch = tmp/100.0;
                break;
            case 'd':
                tmp = (int)strtol(optarg, NULL, 10);
                if (tmp <= 0 || tmp > 100) {
                    fprintf(stderr, "The -d option requires a number from 1 to 100\n");
                    return -1;
                }
                density = tmp/100.0;
                break;
            case 'n': 
                tmp = (int)strtol(optarg, NULL, 10);
                if (tmp < 0 || tmp > 100){
                    fprintf(stderr, "The -n option requires a number from 0 to 100\n");
                    return -1;
                }
                neighbors = tmp/100.0;
                break;
            case 'p':
                tmp = (int)strtol(optarg, NULL, 10);
                if (tmp < 0) {
                    fprintf(stderr, "The -p option requires a number greater than 0\n");
                    return -1;
                }
                print_count = tmp;
                break;
            case 's':
                tmp = (int)strtol(optarg, NULL, 10);
                if (tmp < 5 || tmp > 40) {
                    fprintf(stderr, "The -s option requires a number from 5 to 40\n");
                    return -1;
                }
                size = tmp;
                break;
            case 'l':
                tmp = (int)strtol(optarg, NULL, 10);
                if (tmp < 0 || tmp > 100) {
                    fprintf(stderr, "The -l option requires a number from 0 to 100\n");
                    return -1;
                }
                lightning = tmp/100.0;
                break;
            case 'D':
                tmp = (int)strtol(optarg, NULL, 10);
                if (tmp < 0 || tmp > 100) {
                    fprintf(stderr, "The -D option requires a number from 0 to 100\n");
                    return -1;
                }
                dampness = tmp/100.0;
                break;
            default: 
                fprintf( stderr, "unknown flag caused failure, ignored.\n");
                break;
        }
    }
    char forest[size][size];
    int burned_forest[size][size];
    
    ///dampness affects prob_catch
    ///ex: prob_catch= .3, dampness = .1 --> new prob_catch = .27
    ///100% dampness means nothing will catch fire normally 
    ///0% dampness means that dampness does not have any affect
    prob_catch = prob_catch - (prob_catch*dampness);

    start(forest, burned_forest);
    printf("Cycle: 0\n");
    for (int i=0; i<size; i++){
        for (int j=0; j<size; j++) {
            printf("%c", forest[i][j]);
        }
        printf("\n");
    }
    printf("\n");
        

    if (print_count) {
        printf("\nPrinting %d cycles\n", print_count);
        for (int i=0; i<print_count; i++) {
            printf("Cycle: %d\n", i+1);
            spread(forest, burned_forest);
            int burning = 0;
            for (int n=0; n<size; n++) {
                for (int m=0; m<size; m++) {
                    printf("%c", forest[n][m]);
                    if (forest[n][m] == '*')
                        burning = 1;
                }
                printf("\n");
            }
            printf("s: %d b: %.3f c: %.3f d: %.3f n: %.3f p: %d\n", size, init_burn, prob_catch, density, neighbors, print_count);
            printf("\n");
            if (burning == 0){
                printf("Exiting early, fire completed\n");
                return 0;
            }
        }
    } else {
        clear();
        int burning = 1; 
        while(burning) {
            spread(forest, burned_forest);
            usleep(750000);
            burning = 0;
            for (int r=0; r<size; r++)
                for (int c=0; c<size; c++) {
                    set_cur_pos(r+1, c+1);
                    put(forest[r][c]);
                    if (forest[r][c] == '*')
                        burning = 1;
                }
        }
    return 0;
    }
}
