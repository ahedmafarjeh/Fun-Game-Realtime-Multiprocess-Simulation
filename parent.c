#include "header.h"
int main(int argc,char *argv[])
{	
        signal(SIGUSR1, team1);
        int status;
        int pid;
        int i;
        char str[2];
        int player_number=0;
        int shmID11;
        int shmID12;
        struct player1 *p1;
	struct player1 *p2;
        //create shared memory for bag of player 1 in team 1
        shmID11 = shmget(3000 ,sizeof(struct player1), 0666 | IPC_CREAT);
	if((p1= (struct player1 *)shmat(shmID11, NULL, 0)) == -1) 
		printf ("P1 attached error\n");
	p1->x_position = 0; //initialize for position of player 1 in team 1
	p1->water_amount = 0;//initialize for position of player 1 in team 2
        //create shared memory for bag of player 1 in team 2
        shmID12 = shmget(2000 ,sizeof(struct player1), 0666 | IPC_CREAT);
	if((p2= (struct player1 *)shmat(shmID12, NULL, 0)) == -1) 
		printf ("P2 attached error\n");
	p2->x_position = 0;
	p2->water_amount = 0;
        //create shared memory of tanks 
        key_t key = ftok("str",'b');
        int shmID;
	shmID = shmget(key ,sizeof(struct tanks), 0666 | IPC_CREAT);
	if((tnks= (struct tanks *)shmat(shmID, NULL, 0)) == -1){
		printf("attached error \n");
	} 
	// initalize for tanks
	tnks->tankA=10;
	tnks->tankC=10;
	tnks->tankB=0;
	tnks->tankD=0;
        sleep(3);
        //create semaphore for player 2&3 of team 1
	sem_t *m;
	if((m = sem_open("m_semaphore",O_CREAT,0644,1)) == SEM_FAILED)
	{
		perror("Error");
		exit(1);
	}
	sem_init(m,1,1);
        //create semaphore for player 2&3 of team 2
	sem_t *m1;
	if((m1 = sem_open("m1_semaphore",O_CREAT,0644,1)) == SEM_FAILED)
	{
		perror("Error");
		exit(1);
	}
	sem_init(m1,1,1);
	while(round <= 3){
		for ( i=0; i<WIDTH; i++) {
			if (i == 4){
		        	player_number=0;
		        	}
		         player_number++;
		        switch(pid = child_pids[i] = fork()) {
		        case -1:
		                /* something went wrong */
		                /* parent exits */
		                perror("Parent Soldier fork");
		                exit(1);

		        case 0:
		                sprintf(str, "%d", player_number); // Convert index to string to send it as a parameter to the soldiers main
		                fflush(stdout);
		                execl("./player","./player", (i<4) ? "1" : "2" ,str,(char*) NULL); //Decide upon teams, and create player
		                perror("Exec Error");
		                break; 
		             
		        }
		        sleep(1);
		}
		// send child pids to cilds
		for (i=0; i<WIDTH; i++)
		{
			tnks->pids[i] = child_pids[i+1];	
		}
		// parent tell players to start --> send signal to players
		for ( i=0; i<WIDTH; i++)
		{
		  kill(child_pids[i],SIGUSR1);
		  sleep(1);
		}
		delay1(); // timer for set game time interval
		for(int i=0;i<WIDTH;i++){
		// kill players
		kill(child_pids[i],SIGKILL);
		}
		reset();// reset text color (white)
		printf("Time out\n");
		printf("Tank B = %d\n",tnks->tankB);
		printf("Tank D = %d\n",tnks->tankD);
		// check which team is win in the game based on tank B & D in one round
		if(tnks->tankB > tnks->tankD){
			printf("Team 1 is the winer in round %d\n",round);
			score1++;
		}
		else if(tnks->tankB < tnks->tankD){
			printf("Team 2 is the winer in round %d\n",round);
			score2++;
		}
		else {
			printf(" تعادل في الجولة %d\n",round);
		}
		// parent wait for first players
		if (pid!=0 && pid!=-1) {
		waitpid(child_pids[0], &status,0);
		waitpid(child_pids[4], &status,0);
		}
		// reinitialize shared memory values for new round
		p1->x_position = 0;
		p1->water_amount = 0;
		p2->x_position = 0;
		p2->water_amount = 0;
		tnks->tankA=10;
		tnks->tankC=10;
		tnks->tankB=0;
		tnks->tankD=0;
		// reinitalize player number
		player_number=0;
		//start new round
		round++;
		}
		// decide which team is win in the game
		if(score1 > score2)
			printf("team 1 is the winner\n");
		else 
			printf("team 2 is the winner\n");
		// parent will clean/remove shared memory and semaphore
		if(-1 == (shmctl(shmID, IPC_RMID, NULL))) {
		 	perror("shmctl");
		 }
		 if(-1 == (shmctl(shmID11, IPC_RMID, NULL))) {
		 	perror("shmctl");
		 }
		  if(-1 == (shmctl(shmID12, IPC_RMID, NULL))) {
		 	perror("shmctl");
		 }
		 sem_close(m);
        	 sem_unlink("m_semaphore");
        	 sem_close(m1);
        	 sem_unlink("m1_semaphore");		
	        return 0;
}

int getRandom(int min, int max)
{
	return min + rand() % (max - min);
}
// signal function that calls from parent when he receive SIGUSR1
void team1(int sig){
	reset();// white text color
	// kill players to end the round
	for(int i=0;i<WIDTH;i++){
		kill(child_pids[i],SIGKILL);
		}
		// display destination tanks value
		printf("Tank B = %d\n",tnks->tankB);
		printf("Tank D = %d\n",tnks->tankD);
		// decide who winner
		if(tnks->tankB > tnks->tankD){
			printf("Team 1 is the winer in round %d\n",round);
			score1++;
		}
		else if(tnks->tankB < tnks->tankD){
			printf("Team 2 is the winer in round %d\n",round);
			score2++;
		}
		else {
			printf(" تعادل في الجولة %d\n",round);
		}



}
// timer function
void delay1(){
	for(int i = 15; i > 0; --i) 
	{ 
	usleep(1000000); // one second
	printf("...LOADING : %d \r ",i); 
	fflush(stdout); 
	} 
}


