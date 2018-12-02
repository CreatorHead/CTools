#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>


void error(char *msg);
void handle_shutdown(int sig);

int main(int argc, char *argv[]){
	
	int listener_d = socket(PF_INET, SOCK_STREAM, 0);

	if(listener_d == -1)
	{
		error("Can't open Socket");
	}
	
	//Bind to a Port
	struct sockaddr_in name;
	name.sin_family = PF_INET;
	name.sin_port = (in_port_t)htons(30000);
	name.sin_addr.s_addr = htonl(INADDR_ANY);
	int c = bind (listener_d, (struct sockaddr *) &name, sizeof(name));
	
	if(c == -1)
	{
		error("Can't bind to socket");
	}
	
	//Listen
	if(listen(listener_d, 10) == -1)
	{
		error("Can't listen");
	}
	
	//Accept a Connection
	struct sockaddr_storage client_addr;
	unsigned int address_size = sizeof(client_addr);
	int connect_d = accept(listener_d, (struct sockaddr *)&client_addr, &address_size);
	if(connect_d == -1)
	{
		error("Can't open secondary socket");
	}
	
	//Begin talking
	char *msg = "Internet Knock-Knock Protocol Server\r\nVersion 1.0\r\nKnock! Knock!\r\n";
	if(send(connect_d, msg, strlen(msg), 0) == -1)
	{
		error("send");
	}
}//end of main

int listener_d;
void handle_shutdown(int sig)
{
	if (listener_d)
	close(listener_d);
	fprintf(stderr, "Bye!\n");
	exit(0);
}


void error(char *msg){
	fprintf(stderr,"%s",msg);
}



	
