#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(){
	int server_sockfd, client_sockfd;	//socket file discriptors
	int server_len, client_len;		//lengths of server/client addr structs
	struct sockaddr_un server_addr, client_addr;	//server/client addr structs
	typedef struct{ 
		float x;
		float y;
		int operator;
	 	float result;
	}arithmetic_operatn;
	/* if the file arithmeticServer_socket already exist in the current directory,
	   then remove it */
	unlink("arithmeticServer_socket");	
	/* create the server socket */
	server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);	
	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path, "arithmeticServer_socket");
	server_len = sizeof(server_addr);
	client_len = sizeof(client_addr);
	/* bind server_sockfd to the name arithmeticServer_socket */
	bind(server_sockfd, (struct sockaddr *) &server_addr, server_len);
	/* listen for a max of 5 connections at server_sockfd */
	listen(server_sockfd, 5);
	/* repeatedly accept client connections and communicate with them */
	for(;;){
		arithmetic_operatn operatn;
		printf("server waiting...\n");
		client_sockfd = accept(server_sockfd,
					(struct sockaddr *) &client_addr,
					&client_len);
		//printf("Current client's address %d\n", client_addr.sun_path);
		/* read data from client's socket */
		read(client_sockfd, &operatn, sizeof(arithmetic_operatn));
		/* get right operator and operation */
		switch(operatn.operator){
			case 1:
				operatn.result = operatn.x + operatn.y;
				break;
			case 2:
				operatn.result = operatn.x - operatn.y;
				break;
			case 3:
				operatn.result = operatn.x * operatn.y;
				break;
			case 4:
				operatn.result = operatn.x / operatn.y;
				break;
			case 5:
				operatn.result = (int) operatn.x % (int) operatn.y;
				break;
			default:break;
		};
		/* write modified data back to client's socket */
		write(client_sockfd, &operatn.result, sizeof(float));
    	        /* terminate communication with current client */
		close(client_sockfd);
	}
return 0;
}
