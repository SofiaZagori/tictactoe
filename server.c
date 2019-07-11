#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>



void init_grid(char grid[3][3]);	//arxikopoihsh pinaka
void show_game(char grid[3][3], int game, int wins1, int wins2 ,int draws);
void next_move(char grid[3][3], int *x, int *y, int player);
int check_for_winner(char grid[3][3],int player);



int main(int argc,const char *argv[])	
{
//-------------------------ARXIKOPOIHSH SOCKET-----------------------------------------------
	int psock, sock, x, y;
    	
    	struct sockaddr_in addr; // Internet socket

    	addr.sin_family = AF_INET; // Internet socket
    	addr.sin_addr.s_addr = htonl(INADDR_ANY); // All interfaces 
    	addr.sin_port = htons(atoi(argv[1]));

    	psock = socket(AF_INET, SOCK_STREAM, 0); // Stream socket
    	bind(psock, (struct sockaddr *)&addr, sizeof(addr));
    	listen(psock, 2);
//---------------------------ARXIKOPOIHSH TRILIZAS-------------------------------------------
	char grid[3][3],ans;
	int game,wins1,wins2,draws,turns,flag;
	
	int pos[2];
	game=1;
	wins1=0;
	wins2=0;
	draws=0;
	
	int click=10;
	
	
	while(click==10){
                     
                     
		flag=-1;
		turns=1;

		init_grid(grid);
		show_game(grid,game,wins1,wins2,draws);
		sock = accept(psock, NULL, NULL);
		while(turns<=9 && flag==-1){ //dhladh den exoyme kapoion prowro nikhth
			
			if(turns%2==1){		//an o gyros einaai perittos paizei o player1 
//----------------------RECV SOCKET POS FROM CLIENT--------------------------------------------------
				
 			       printf("connection established...\n");
				recv(sock, &x, sizeof(int), 0);
				recv(sock, &y, sizeof(int), 0);
				printf("[%d]\n", x);
				printf("[%d]\n", y);
				
				grid[x][y]='X';
				flag=check_for_winner(grid,1);
				
        			printf("\nconnection closed...\n");
			}
			else{
				next_move(grid,&x, &y,1);
//----------------------SENT SOCKET POS TO CLIENT----------------------------------------------------
				send(sock, &x, sizeof(int), 0);
				send(sock, &y, sizeof(int), 0);

				grid[x][y]='O';
				flag=check_for_winner(grid,2);
			}
			show_game(grid,game,wins1,wins2,draws);
			turns++;
		}
		game++;
		if(flag==-1){
			printf("\n isopaliiiaaaaaaa :D \n");
			draws++;
		}
		else if(flag==1){
			printf("\n player 1 is the winner\n");
			wins1++;
		}
		else{
			printf("\n player 2 is the winner\n");
			wins2++;
		}


// Edw na ginei ena recv gia to SIGNAL!!!

       recv(sock, &click, sizeof(int), 0);


//------------------------KLEISIMO SOCKET----------------------------------------------------------
		close(sock);
		
	} // #END OF WHILE
	

	close(psock);
	unlink(argv[1]);

	return 0;
}

// --------------- apo dw ksekinane oi synarthseis

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
	printf("\n\t TICTACTOE SERVER\n");
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


void next_move(char grid[3][3], int *x, int *y, int player) {
	int flag= 0;

	while(!flag){
		
		printf("player %d\n",player);
		
		*x= rand()%3; //kanei random kinhhsh 
		*y= rand()%3;
		
		if(grid[*x][*y]==' '){
			flag=1;	
		}	
	}
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
	if(counter==3){ //gia kapoion agnwsto logo otan htan counter==2 stamatouse nwris 
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
		j--; //gia na diasxizei thn diaagwnio kathws proxwra grammes 
	}
	if(counter==2){
		return player;
	}
	return -1;


}

