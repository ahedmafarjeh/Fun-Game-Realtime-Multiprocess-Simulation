#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <signal.h>
#include <wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>  
#include <stdint.h>
#include <sys/mman.h>
#define WIDTH 8
void team1(int sig);
void delay1();
void startGame(int);
void startAttack(int);
void filling_tank();
void stoleWater();
void reset() {
        printf("\033[0m");
}
void yellow() {
   printf("\033[1;33m"); } 
void green () {
   printf("\033[0;32m"); 
} 
void red () { 
   printf("\033[1;31m"); 
}
void blue () { 
   printf("\033[94m"); 
}
void orange () { 
   printf("\033[93m"); 
}
void magenta () { 
   printf("\033[95m"); 
}
int team;
int player_number;
struct player1{
int x_position; // 0--A OR C/ 1 -- in the way/ 2--B OR D
int water_amount;// represent bag of water player 1
};
struct player1 *p1;
struct player1 *p2;
struct tanks{
int tankA; 
int tankB;
int tankC;
int tankD;
int pids[8];
};
int score1=0;
int score2=0;
 int round=1;
int child_pids[WIDTH];
struct tanks *tnks ;
int flag=1;
