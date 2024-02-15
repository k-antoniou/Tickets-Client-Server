#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
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
    int sock_fd, i; 
    struct sockaddr_in servaddr;
	int requested_match_id, requested_tickets, success = 0;
	struct match* football_match;
	float price;
	//aloccate football_match array for 10 entries
	football_match = (match *)malloc(sizeof(match) * 10);
	for(i=0; i<10; i++){
	    football_match[i].team1 = (char*) malloc(sizeof(char) * 120);
		football_match[i].team2 = (char*) malloc(sizeof(char) * 120);
	}

    //create SOCK_STREAM socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    bzero(&servaddr, sizeof(servaddr));
 	//initialize socket
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(9057);
	
	//set 127.0.0.1 as IP address
    inet_pton(AF_INET,"127.0.0.1",&(servaddr.sin_addr));
	//connect to the corresponding server
    connect(sock_fd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	//get random values
	srand(time(0));
	requested_match_id = (rand() % 10) + 1;
	requested_tickets = (rand() % 3) + 1;
	//write values to server
	write(sock_fd, &requested_match_id, sizeof(requested_match_id));
   	write(sock_fd, &requested_tickets, sizeof(requested_tickets));
	//read from server
    read(sock_fd, &success, sizeof(success));
	if(success){
		read(sock_fd, &price, sizeof(price));
		printf("Ticket(s) purchased successfully.\n Ticket(s) price: %f\n", price);
	}else{
		printf("The number of available tickets is less than %d\n", requested_tickets);
	}
	close(sock_fd);
    return 0;
}
