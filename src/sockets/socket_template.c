#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>


void error(char *msg);
int open_listener_socket();
void bind_to_port(int socket, int port);
int say(int socket, char *s);
void handle_shutdown(int sig);
int read_in(int socket, char *buf, int len);
int catch_signal(int sig, void (*handler)(int));

int listener_d;
int main(int argc, char *argv[]) {
	return 0;
}
void error(char *msg){
	fprintf(stderr,"%s: %s\n", msg, strerror(errno));
	exit(1);
}

int open_listener_socket()
{
	int s = socket(PF_INET, SOCK_STREAM, 0);
	if(s == -1) {
		error("Can't open Socket");	
	}
}


void bind_to_port(int socket, int port){
	struct sockaddr_in name;
	name.sin_family = PF_INET;
	name.sin_port = (in_port_t)htons(30000);
	name.sin_addr.s_addr = htonl(INADDR_ANY);
	int reuse = 1;
	if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(int)) == -1)
	error("Can't set the reuse option on the socket");
	int c = bind (socket, (struct sockaddr *) &name, sizeof(name));
	if (c == -1)
	error("Can't bind to socket");
}

int say(int socket, char *s)
{
	int result = send(socket, s, strlen(s), 0);
	if (result == -1)
	fprintf(stderr, "%s: %s\n", "Error talking to the client", strerror(errno));
	return result;
}



void handle_shutdown(int sig)
{
	if (listener_d)
	close(listener_d);
	fprintf(stderr, "Bye!\n");
	exit(0);
}


int read_in(int socket, char *buf, int len)
{
	char *s = buf;
	int slen = len;
	int c = recv(socket, s, slen, 0);
		while ((c > 0) && (s[c-1] != '\n')) {
			s += c; slen -= c;
			c = recv(socket, s, slen, 0);
		}
	if (c < 0)
		return c;
	else if (c == 0)
		buf[0] = '\0';
	else
		s[c-1]='\0';
	return len - slen;
}

int catch_signal(int sig, void (*handler)(int))
{
	struct sigaction action;
	action.sa_handler = handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	return sigaction (sig, &action, NULL);
}


