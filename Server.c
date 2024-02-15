#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <time.h>

typedef struct match {
	int match_id;
	char* team1;
	char* team2;
	int available_tickets;
	float price;
}match;

int main(int argc, char* argv[])
{
	
	int success = 0, pipefd[2], sold_tickets = 0 , p_id, child_id;
    int listen_fd, client_fd, i = 0,j, requested_match_id, requested_tickets;
    struct sockaddr_in servaddr; 
	struct match* football_match;
	float price;

	//aloccate football_match array for 10 entries
	football_match = (match *)malloc(sizeof(match) * 10);
	//initialize football_match array
	srand(time(0));
	for(i=0;i<10; i++){
		football_match[i].match_id = i+1;
	    football_match[i].team1 = (char*) malloc(sizeof(char) * 120);
		football_match[i].team2 = (char*) malloc(sizeof(char) * 120);
		football_match[i].available_tickets = (rand() % 5) + 1;
	}
	football_match[0].team1="panaigialios";
	football_match[0].team2="panachaiki";
	football_match[1].team1="pao";
	football_match[1].team2="aek";
	football_match[2].team1="paok";
	football_match[2].team2="pao";
	football_match[3].team1="pas.giannena";
	football_match[3].team2="aek";
	football_match[4].team1="aek";
	football_match[4].team2="aris";
	football_match[5].team1="aris";
	football_match[5].team2="paok";
	football_match[6].team1="paok";
	football_match[6].team2="aek";
	football_match[7].team1="aek";
	football_match[7].team2="osfp";
	football_match[8].team1="pao";
	football_match[8].team2="osfp";
	football_match[9].team1="osfp";
	football_match[9].team2="lamia";
	football_match[0].price=10.0;
	football_match[1].price=12.4;
	football_match[2].price=50.3;
	football_match[3].price=23.7;
	football_match[4].price=11.1;
	football_match[5].price=21.5;
	football_match[6].price=67.2;
	football_match[7].price=32.1;
	football_match[8].price=72.0;
	football_match[9].price=45.8;
	//print football match array
	for(i=0;i<10; i++){
		printf("MatchID: %d\n", football_match[i].match_id);
	    printf("Team 1: %s\n", football_match[i].team1);
		printf("Team 2: %s\n", football_match[i].team2);
		printf("Available tickets: %d\n", football_match[i].available_tickets);
		printf("Price: %f\n", football_match[i].price);
		printf("\n");
	}

	//create SOCK_STREAM socket
    listen_fd = socket(AF_INET, SOCK_STREAM, 0); 
 
    bzero(&servaddr, sizeof(servaddr));
 	
 	//init socket
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY); 
    servaddr.sin_port = htons(9077); 
	
	//bind to IP and port
    bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    
	printf("\tthe SERVER is working\n");
	printf("\twaiting for clients to connect...\n");	 
	printf("\t========================\n");
	
	i=0;
	listen(listen_fd, 1);
	while(1)
    {
		//wait for the processes to finish
		wait(NULL);
		//accept and connect to client
		client_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);
		printf("This is client %d \n", ++i);
		printf("Connected\n");
		//create pipe
		pipe(pipefd);
		if((p_id = fork())==0){
			//read the requested match id and number of tickets from server
			read(client_fd, &requested_match_id, sizeof(requested_match_id));
        	read(client_fd, &requested_tickets, sizeof(requested_tickets));
			printf("Requested match id: %d \n", requested_match_id);
			printf("Requested tickets: %d \n", requested_tickets);
			printf("Available tickets: %d \n", football_match[requested_match_id-1].available_tickets);
			//check if there are enough tickets for the requested match
			if(requested_tickets <= football_match[requested_match_id-1].available_tickets){
				success = 1;
			}
			write(client_fd, &success,sizeof(success));
			price = requested_tickets * football_match[requested_match_id-1].price;
			if(success){
				write(client_fd, &price,sizeof(price));
				write(pipefd[1], &requested_match_id, sizeof(requested_match_id));
				write(pipefd[1],&requested_tickets,sizeof(requested_tickets));
			}
			success=0;
		
			close(client_fd);
			//terminate child
			exit(0);
		}else{
			//parent process reads from pipe
			read(pipefd[0],&requested_match_id,sizeof(requested_match_id));
			read(pipefd[0],&sold_tickets,sizeof(sold_tickets));
			football_match[requested_match_id-1].available_tickets=football_match[requested_match_id-1].available_tickets - sold_tickets;
			//print updated football match array
			for(j=0; j<10; j++){
				printf("MatchID: %d\n", football_match[j].match_id);
	    		printf("Team 1: %s\n", football_match[j].team1);
				printf("Team 2: %s\n", football_match[j].team2);
				printf("Available tickets: %d\n", football_match[j].available_tickets);
				printf("Price: %f\n", football_match[j].price);
				printf("\n");
			}	
		}
	}
    return 0;
}

