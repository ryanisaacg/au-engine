#include "au.h"

#include <stdio.h>

int main() {
	au_init_headless();
	char buffer[512];
	printf("Run as server? [Y/n]: ");
	fgets(buffer, 512, stdin);
	//Run as server or client
	if(buffer[0] == 'Y') {
		AU_Socket server = au_socket_server_new(8888);
		AU_Socket client = au_socket_server_accept(server);
		au_socket_tcp_recv(client, buffer, 512);
		printf("Message received: %s\n", buffer);
	} else {
		printf("Enter the address of the server to connect to:\n");
		fgets(buffer, 512, stdin);
		AU_Socket client = au_socket_connection_new(buffer, 8888);
		printf("Enter a string:\n");
		fgets(buffer, 512, stdin);
		au_socket_tcp_send(client, buffer, 512);
		printf("Message sent: %s\n", buffer);
	}
	return 0;
}
