#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include<signal.h>

static void handler(int sig);
void init_grid(char grid[3][3]);	//initialization of array 
void show_game(char grid[3][3], int game, int wins1, int wins2 ,int draws);
void next_move(char grid[3][3], int *x, int *y, int player);
int check_for_winner(char grid[3][3],int player);



int main(int argc,const char *argv[])	
{
//-------------------ARXIKOPOIHSH SOCKET---------------------------------------------------
	int sock, x,y;
   	struct sockaddr_in addr; // Internet socket
	char grid[3][3],ans;
	int game,wins1,wins2,draws,turns,flag;

    	addr.sin_family = AF_INET; // Internet socket
    	inet_aton(argv[1], &addr.sin_addr); 
    	addr.sin_port = htons(atoi(argv[2])); 

    	sock = socket(AF_INET, SOCK_STREAM, 0); // Stream socket
    	connect(sock, (struct sockaddr *)&addr, sizeof(addr));

//--------------------ARXIKOPOIHSH TRILIZAS-----------------------------------------------
	
	game=1;
	wins1=0;
	wins2=0;
	draws=0;


	
	do{     
                int *click;
		flag=-1;
		turns=1;

		init_grid(grid);
		show_game(grid,game,wins1,wins2,draws);

		while(turns<=9 && flag==-1){  //while we don't have an early winner 
			if(turns%2==1){		//if the number of the round is odd, it's  player1's turn  
				next_move(grid, &x, &y, 1);
				printf("after next_move");
//--------------------SEND SOCKET POS TO SERVER-------------------------------------------------
				send(sock, &x, sizeof(int), 0);
				send(sock, &y, sizeof(int), 0);
				grid[x][y]='X';
				flag=check_for_winner(grid,1);
			}
			else{
//---------------------RECV SOCKET POS FROM SERVER----------------------------------------------

				recv(sock, &x, sizeof(int), 0);
				recv(sock, &y, sizeof(int), 0);
				grid[x][y]='O';
				flag=check_for_winner(grid,2);
			}
			show_game(grid,game,wins1,wins2,draws);
			turns++;
		}
		game++;
		if(flag==-1){
			printf("\n You are both winners ... *lol no LOoSERs* :D \n");
			draws++;
		}
		else if(flag==1){
			printf("\n player 1 is the winner\n");
			wins1++;
		}
		else{
			printf("\n Computer/server is the winner\n");
			wins2++;
		}

		//printf("\n\n new game?(y/n) \n");
		//scanf(" %c",&ans); //a space added so the enter is not taken as a character


		printf("\n\n if you want the game to stop completely, click Ctrl+c \n");

  // we send the signal

 void handler(int sig) {
	write(STDOUT_FILENO,"\n Caught signal for Ctrl+C \n",11);
	int click;
	printf("\n\n give 1 \n");
	scanf("%d",&click); 

	send(sock, &click, sizeof(int), 0);    // we send that to the server

}

    	struct sigaction act = { {0} };
    
    	act.sa_handler = handler;
    	sigaction(SIGINT,&act,NULL);
    
    	while (1) {	// A big pause to assign a signal to our process
    		pause();
    	}

	}while(1); // END OF WHILE 
	
	
	

	close(sock);

	return 0;

}

// FUNCTIONS START



void init_grid(char grid[3][3]){
	int i,j;
	for(i=0; i<3; i++){
		for(j=0; j<3; j++){
			grid[i][j]=' ';			
		
		}	
	}
}
void show_game(char grid[3][3], int game, int wins1, int wins2 ,int draws){
	int i,j;
	printf("\n\t TICTACTOE CLIENT\n");
	printf("\t\t game %d \n\n",game);
	printf(" player1\t\tplayer\n");
	printf("Wins1: %d \t draws:%d \t Wins2:%d \n",wins1,draws,wins2);
	for(i=0; i<3; i++){
		printf("\t\t");
		for(j=0; j<3; j++){
			if(j<2){
				printf("%c | ",grid[i][j]);			
			}
			else{
				printf("%c",grid[i][j]);	
			}
		}
		if(i<2){		
			printf("\n\t\t---------\n");
		}
		
	}
	printf("\n\n");
	
}

void next_move(char grid[3][3], int *x, int *y, int player){
	int flag= 0;

	while(flag==0){
		
		printf("player %d\n", player);
		if(player==1){
			printf("\nYOU ARE X\n");
			scanf("%d,%d",x,y);
			if(*x<3 && *y<3){
				if(grid[*x][*y]==' '){
					flag=1;	
				}
			}
		}
	}
	printf("into the next_move\n");
}

int check_for_winner(char grid[3][3],int player){
	int i,j,counter;
	char s;
	
	counter=0;	
	i=0;					//grammes
	while(i<3 && counter !=2){
		counter=0;
		s=grid[i][0];
		for(j=1;j<3;j++){
			if(s==grid[i][j] && grid[i][j]!=' '){
				counter++;
			}
		
		}
		i++;
	}
	
	if(counter==2){
		return player;
	}
	j=0;					//sthles
	while(j<3 && counter !=2){
		counter=0;
		s=grid[0][j];
		for(i=1;i<3;i++){
			if(s==grid[i][j] && grid[i][j]!=' '){
				counter++;
			}
		
		}
		j++;
	}
	if(counter==2){
		return player;
	}
	//diagwnios
	counter =0;
	s=grid[0][0];
	for(i=0;i<3;i++){
		if(s==grid[i][i] && grid[i][i]!=' '){
			counter++;
		}	
	}
	if(counter==3){
		return player;
	}
	//deyterh diagwnios
	counter=0;
	s=grid[0][2];
	j=1;
	for(i=1;i<3;i++){
		if(s==grid[i][j] && grid[i][j]!=' '){
			counter++;
		}
		j--; //to check the diagonal lines, as the lines progress 
	}
	if(counter==2){
		return player;
	}
	return -1;


}

