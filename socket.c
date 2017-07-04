#include "socket.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Platform specific includes and defines to provide a universal socket API
//Use WinSock on Win32 platforms and use BSD sockets on everything else
//Define an alias for shutdown reading and writing
//Define close as a macro so closesocket and close look the same
#ifdef _WIN32
	#include <winsock2.h> 
	#include <ws2tcpip.h>
	#define SHUTDOWN_OPTION SD_BOTH
	#define CLOSE closesocket
#else
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <netinet/ip.h> 
	#include <arpa/inet.h>
	#include <unistd.h>
	#define SHUTDOWN_OPTION SHUT_RDWR
	#define CLOSE close
#endif

static int new_socket(bool is_udp) {
	int id = socket(AF_INET, is_udp ? SOCK_DGRAM : SOCK_STREAM, 0);
	if(id < 0) {
		fprintf(stderr, "Failed to create %s socket\n", is_udp ? "UDP" : "TCP");
		exit(1);
	}
	return id;
}

static int bind_socket(int id, int port) {
	struct sockaddr_in name;
	memset(&name, 0, sizeof(name));
	name.sin_family = AF_INET;
	name.sin_port = port;
	name.sin_addr.s_addr = INADDR_ANY;
	int return_val = bind(id, (struct sockaddr*) &name, sizeof(name));
	if(return_val != 0) {
		fprintf(stderr, "Failed to bind a socket to port %d\n", port);
		exit(1);
	}
	listen(id, 5);
	return id;
}

static int connect_socket(int id, const char* name, int port) {
	struct sockaddr_in serv_addr;
	memset(&(serv_addr), 0, sizeof(struct sockaddr_in));
	serv_addr.sin_addr.s_addr = inet_addr(name);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = port;
	int return_val = connect(id, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	if(return_val != 0) {
		fprintf(stderr, "Failed to connect to a remote socket at address %s and port %d\n", name, port);
		exit(1);
	}
	return id;
}

AU_Socket au_socket_server_new(int port) {
	return (AU_Socket) { bind_socket(new_socket(false), port), bind_socket(new_socket(true), port), true };
}

AU_Socket au_socket_server_accept(AU_Socket server) {
	struct sockaddr_in serv_addr;
	socklen_t serv_size = sizeof(serv_addr);
	int socket = accept(server.tcp, (struct sockaddr*) &serv_addr, &serv_size);
	int udp = new_socket(true);
	int return_val = bind(udp, (struct sockaddr*) &serv_addr, serv_size);
	if(return_val != 0) {
		fprintf(stderr, "Failed to accept a connection\n");
		exit(1);
	}
	return (AU_Socket) { socket, udp, false };
}

AU_Socket au_socket_connection_new(const char* remote_address, int port) {
	return (AU_Socket) { connect_socket(new_socket(false), remote_address, port), connect_socket(new_socket(true),remote_address, port), false };

}

void au_socket_tcp_send(AU_Socket socket, const void* message, size_t length) {
	send(socket.tcp, message, length, 0);
}

size_t au_socket_tcp_recv(AU_Socket socket, void* message, size_t buffer_length) {
	return recv(socket.tcp, message, buffer_length, 0);
}

void au_socket_udp_send(AU_Socket socket, const void* message, size_t length) {
	send(socket.udp, message, length, 0);
}

size_t au_socket_udp_recv(AU_Socket socket, void* message, size_t buffer_length) {
	return recv(socket.udp, message, buffer_length, 0);
}

void au_socket_close(AU_Socket socket) {
	shutdown(socket.tcp, SHUTDOWN_OPTION);
	shutdown(socket.udp, SHUTDOWN_OPTION);
	CLOSE(socket.tcp);
	CLOSE(socket.udp);
}
