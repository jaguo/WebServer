#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h> 	// sockaddr_in htons(),htonl(),ntohs(),ntohl()
#include <sys/types.h>  	// socket
#include <sys/socket.h> 	// socket
#include <string.h>			// bzero
#include <arpa/inet.h>
#include <netdb.h>

//htons()"host to net short",htonl()"host to net long"

/*
int socket(int domain, int type,int protocol)
int bind(int sockfd, struct sockaddr *my_addr, int addrlen)
int listen(int sockfd,int backlog)
int accept(int sockfd, struct sockaddr *addr,int *addrlen)
*/

void headers(int client);

int main(int argc, char const *argv[])
{
	int server_fd,client;
	int portnumber, sin_size;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	char hello[] = "Hello World!";

	if(argc != 2)
	{
		fprintf(stderr, "Port Error: %s\n", argv[1]);
		exit(1);
	}

	if((portnumber = atoi(argv[1])) < 0)
	{
		fprintf(stderr, "Prot Error: %s\n", argv[1]);
		exit(1);
	}

	if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		fprintf(stderr, "Socked Error\n");
		exit(1);
	}

	//
	bzero(&server_addr,sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(portnumber);

	if(bind(server_fd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) == -1)
	{
		fprintf(stderr, "Bind Error: %s\n", strerror(errno));
		exit(1);
	}

	if(listen(server_fd, 5) == -1)
	{
		fprintf(stderr, "Listen Error: %s\n", strerror(errno));
		exit(1);
	}

	while(1)
	{
		sin_size = sizeof(struct sockaddr_in);
		if((client = accept(server_fd, (struct sockaddr *)(&client_addr), &sin_size)) == -1)
		{
			fprintf(stderr, "Accept Error: %s\n", strerror(errno));
			exit(1);
		}

		fprintf(stdout,"Server get connection from %s\n",inet_ntoa(client_addr.sin_addr));

		// if(write(new_sockfd, hello, sizeof(hello)) == -1)
		// {
		// 	fprintf(stderr, "Write Error: %s\n", strerror(errno));
		// 	exit(1);
		// }
		headers(client);

		send(client, hello, sizeof(hello), 0);

		close(client);
	}

	close(server_fd);

	/* code */
	return 0;
}

void headers(int client)
{

	char buf[1024];

	strcpy(buf, "HTTP/1.0 200 OK\r\n");
	send(client, buf, strlen(buf), 0);
	strcpy(buf, "Server: MyServer/0.1.0\r\n");
	send(client, buf, strlen(buf), 0);
	sprintf(buf, "Content-Type: text/html\r\n");
	send(client, buf, strlen(buf), 0);
	strcpy(buf, "\r\n");
	send(client, buf, strlen(buf), 0);
}