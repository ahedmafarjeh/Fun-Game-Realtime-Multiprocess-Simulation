#include "header.h"
void main(int argc,char *argv[]){
	signal(SIGUSR1, startGame);
	team = strtol(argv[1], NULL, 10);
	player_number = strtol(argv[2], NULL, 10);
	
	
	
	// attached to shared memory of tanks 
	int shmID;
	key_t key = ftok("str",'b');
	shmID = shmget(key ,sizeof(struct tanks),0 );
	if((tnks= (struct tanks *)shmat(shmID, NULL, 0)) == -1){
		printf("attached error tnks \n");
	        }
	//attached to shared memory for player 1 in team 1 
	if(player_number == 1){
		if(team == 1){
		int shmID11;
		shmID11 = shmget(3000 ,sizeof(struct player1), 0);
		if((p1= (struct player1 *)shmat(shmID11, NULL, 0)) == -1) 
		printf ("P1 attached error\n");
		}
		//attached to shared memory for player 1 in team 2
		else if (team == 2 ){
		int shmID12;
		shmID12 = shmget(2000 ,sizeof(struct player1), 0);
		if((p2= (struct player1 *)shmat(shmID12, NULL, 0)) == -1) 
		printf ("P2 attached error\n");
		
		}
	}

	if (player_number == 2 || player_number == 3){
		signal(SIGUSR2, startAttack);
	}

	printf("player number is %d in team %d\n",player_number,team);
	
	while(flag){};
	exit(1);

 }
 
void startGame(int sig){
	
	//first player
	if (player_number == 1){
		filling_tank(); 
	}

	//second & third plyer
	else if (player_number == 2 || player_number == 3 ) {
	  	stoleWater();
	  	}
	  
	// fourth player 
	else {
	 	putSand();
	}
	flag=0;

}
// filling tank B  by P11
void filling_tank(){
  srand(getpid());
  if (team == 1){
	while(tnks->tankA > 0){
		p1->x_position = 0;
		red();
		printf("i am at position A\n");
		sleep(1);
		red();
		printf("tank A size before P11 take water --> %d\n",tnks->tankA);
		if (tnks->tankA < 3){
		
		p1->water_amount=tnks->tankA;
		tnks->tankA -= tnks->tankA;
		}
		else {
		tnks->tankA -= 3;
		p1->water_amount=3;
		}
		red();
		printf("P11 water amount is %d\n",p1->water_amount);
		red();
		printf("tank A size after P11 take water --> %d\n ",tnks->tankA);
		p1->x_position = 1;
		red();
		printf("P11 in the way\n");
		sleep(2);
		int is_accident = (rand() % (1 - 0 + 1)) + 0;
		if (is_accident == 0)
			tnks->tankB += p1->water_amount;
		else {
			red();
			printf("P11 had an accident\n");
			int loosing_water = getRandom(2,1);
			if (p1->water_amount < loosing_water){
				loosing_water = p1->water_amount;
			}
			p1->water_amount = p1->water_amount-loosing_water;
			red();
                       printf("losing amount in bag1 of water is--> %d\n" ,loosing_water);
                       red();
                       printf("amount of water in bag1  is--> %d\n" ,p1->water_amount);
			tnks->tankB += p1->water_amount ;
			}
		red();	
		printf("\n i filled tank B by my bag\n");
		red();
		printf("tank B size %d\n",tnks->tankB);
		p1->x_position = 2;// At pos B
		sleep(2);
		}
		red();
		printf("i am P11 at end\n");
		
	}
else {
       // filling tank C  by P21
	while(tnks->tankC > 0){
		p2->x_position = 0;
		yellow();
		printf("i am at position C\n");
		sleep(1);
		yellow();
		printf("tank C size before P21 take water --> %d\n",tnks->tankC);
		if (tnks->tankC < 3){
		p2->water_amount=tnks->tankC;
		tnks->tankC -= tnks->tankC;
		}
		else {
		tnks->tankC-= 3;
		p2->water_amount=3;
		}
		yellow();
		printf(" P21 water amount is --> %d\n",p2[0].water_amount);
		yellow();
		printf("tank C size after P21 take water --> %d\n",tnks->tankC);
		p2->x_position = 1;
		yellow();
                printf("P21 in the way\n");
		sleep(2);
		int loosing_water = getRandom(2,1);
		int is_accident = (rand() % (1 - 0 + 1)) + 0;
		if (is_accident == 0)
			tnks->tankD += p2->water_amount;
		else {
			yellow();
			printf("P21 had an accident\n");
			
			if (p2->water_amount < loosing_water){
				loosing_water = p2->water_amount;
			}
			yellow();
			printf("losing amount in bag2 water is %d\n",loosing_water);
			yellow();
			printf("amount of water in bag2  is--> %d\n" ,p2->water_amount);
			p2->water_amount = p2->water_amount - loosing_water;
			tnks->tankD += p2->water_amount;
			}	
		yellow();
		printf("i filled tank D by my bag\n");
		yellow();
		printf("tank D size %d\n",tnks->tankD);
		p2->x_position = 2;// At pos D
		sleep(2);
		}
		yellow();
		printf("i am P21 at end\n");
		
	}
	kill(getppid(),SIGUSR1);

}
void stoleWater(){
      sleep(3);
      green();//green color
      srand (time(0));
      if (team == 2){
       green();
	printf("the bags in second and third players are empty\n");
	//attached to shared memory of P11 for player 2 &3 in team 2 
	int shmID11 = shmget(3000 ,sizeof(struct player1), 0);
	if((p1= (struct player1 *)shmat(shmID11, NULL, 0)) == -1) 
	printf ("P1 attached error \n");
	

	sem_t *m;
	sem_t *p;
	
	//create semaphore for player 2&3 of team 2
	if((m = sem_open("m_semaphore",O_CREAT,0644,1)) == SEM_FAILED)
	{
		perror("Error");
		exit(1);
	}
	
	//create semaphore for player 2&3 of team 2
	
	if((p = sem_open("p_ysemaphore",O_CREAT,0644,1)) == SEM_FAILED)
	{
		perror("Error");
		exit(1);
	}
	
	
	while (tnks->tankA > 0){
		if (player_number == 2){
		     sem_wait(m);
		     green();
		     printf("i am second player in team 2, i aquire key\n");  
		     }
		 else {
		     sem_wait(m);
		     green();
		     printf("i am third player in team 2, i aquire key\n");   
		     }
                // P22 and P23 trying to stole from P11
                green();
		printf("team 2 see team 1 in postion is %d\n",p1->x_position);
		if (p1->x_position == 1){
			green();
			printf("i am inside stole water \n");
			int stoolen_amount = getRandom(2,1);
			
                       if (p1->water_amount < stoolen_amount ) {
                       stoolen_amount = p1->water_amount;
                       }
			p1->water_amount -= stoolen_amount;
			tnks->tankA += stoolen_amount;
			green();
			printf("water amount of team 1 %d\n", p1->water_amount);
			green();
			printf("team 1 had stoolen by team2 with stoolen water %d\n",stoolen_amount );
			green();
			printf("tank A %d\n", tnks->tankA);
			}
	
		if (player_number == 2){
		     sem_post(m);
		     green();
		     printf("i am second player in team 2, i relase key for third player\n");
		     }
		 else {
		     sem_post(m);
		     green();
		     printf("i am third player in team 2, i relase key for second player\n");   
		     }
		
		sleep(2);
	}
     }
     
    
else{ 	
	blue();
	printf("the bags of second and third plyer of team 1 are empty\n");
	//attach to shared memoy of palyer 1 in team 2 for player 2 &3 in team 1 
	int shmID12 = shmget(2000 ,sizeof(struct player1), 0);
	if((p2= (struct player1 *)shmat(shmID12, NULL, 0)) == -1) 
	printf ("P2 attached error\n");
	//create semaphore for player 2&3 of team 1
	sem_t *m1;
	sem_t *p1;
	if((m1 = sem_open("m1_semaphore",O_CREAT,0644,1)) == SEM_FAILED)
	{
		perror("Error");
		exit(1);
	}
	
	
	//create semaphore for player 2&3 of team 1
	
	if((p1 = sem_open("p1_ysemaphore",O_CREAT,0644,1)) == SEM_FAILED)
	{
		perror("Error");
		exit(1);
	}
	
	while (tnks->tankC > 0){
		if (player_number == 2){
		     sem_wait(m1);
		     blue();
		     printf("i am second player in team 1, i aquire key\n");  
		     }
		 else {
		     sem_wait(m1);
		     blue();
		     printf("i am third player in team 1, i aquire key\n");   
		     }
		//sleep(2);
                // P12 and P13 trying to stole from P21
                blue();
		printf(" team 1 see team 2 in postion is %d\n",p2->x_position);
		if (p2->x_position == 1){
			int stoolen_amount = getRandom(2,1);
                        if (p2->water_amount < stoolen_amount) {
			//p1->water_amount -= stoolen_amount;
			stoolen_amount = p2->water_amount;
			}
			p2->water_amount -= stoolen_amount;
			tnks->tankC += stoolen_amount;
			blue();
			printf("team 2 had stoolen by team1 with stoolen water %d\n",stoolen_amount );
			blue();
	                printf("water amount of team 2 %d\n", p2->water_amount);
	                blue();
			printf("tank C %d\n", tnks->tankC);
			}
		if (player_number == 2){
		     sem_post(m1);
		     blue();
		     printf("i am second player in team 1, i relase key for third player\n");
		     }
		 else {
		     sem_post(m1);
		     blue();
		     printf("i am third player in team 1, i relase key for second player\n");   
		     }
		sleep(2);
	}
 }


}
// player 4 start helping player1 
void putSand(){
	orange();
	printf("i am player 4\n");
	// check which is team ?
        // p14 trying to put sand in P22 & P23 bags 
	if(team == 1){
		while (tnks->tankA > 0){
		int attacked_num = getRandom(3,2);
		if (attacked_num == 3){
				orange();
				printf("i put sand in bag3 of team 2\n");
				kill(tnks->pids[6],SIGUSR2);
				
			}
		else {
				orange();
				printf("i put sand in bag2 of team 2\n");
				kill(tnks->pids[5],SIGUSR2);
				
			}
			sleep(4);
			}
			
		
		
	}
        // p24 trying to put sand in P12 & P13 bags
	else {
		magenta();
		while (tnks->tankC > 0){
		int attacked_num = getRandom(3,2);
		if (attacked_num == 3){
				magenta();
				printf("i put sand in bag3 of team 1\n");
				kill(tnks->pids[2],SIGUSR2);
				
			}
		else {
				magenta();
				printf("i put sand in bag2 of team 1\n");
				kill(tnks->pids[1],SIGUSR2);
				
			}
			sleep(4);
			}

		
	
	}
     	
	
}
     
void startAttack(int sig){
	printf("player number is %d in team %d\n",player_number, team);
	int isDefend = getRandom(1,0);
        // player 2 & 3 failed to defend each other from player 4 attack
	if(isDefend == 0){
        // player 4 slowing player 2 down 
	if (player_number == 2){
		printf("p2 will sleep\n");
		sleep(3);
		if (team == 2)
			printf("p2 team 2 after sleep\n");
		else 
			printf("p2 team 1 after sleep\n");
	}
        // player 4 slowing player 3 down 
	else  {
		printf("p3 will sleep\n");
		sleep(3);
		if (team == 2)
			printf("p3 team 2 after sleep\n");
		else 
			printf("p3 team 1 after sleep\n");
	}
	
	}
	// player 2 & 3 defend each other from player 4 attack
	else {
		
		if (player_number == 2){
		if(team == 1)
			printf("team 1: p3 defends p2  \n");
		else 
			printf("team 2: p3 defends p2  \n");
		sleep(1);
		
	}
	else  {
	if(team == 1)
			printf("team 1: p2 defends p3  \n");
		else 
			printf("team 2: p2 defends p3  \n");
		sleep(1);
		
	}

}
}


int getRandom(int max, int min)
{
   srand(time(0));
   return (rand() % (max - min + 1)) + min;
}

