#pragma once

#include <stdbool.h>
#include <stddef.h>

typedef struct {
	int tcp, udp;
	bool is_server;
} AU_Socket;

//Create a new server listening on a given port
AU_Socket au_socket_server_new(int port);
//Accept a connection on a server socket (blocks)
AU_Socket au_socket_server_accept(AU_Socket server);
//Create a new client socket
AU_Socket au_socket_connection_new(const char* remote_address, int port);
//Send a message with a given length over TCP
void au_socket_tcp_send(AU_Socket socket, const void* message, size_t length);
//Receive data from TCP and return the amount of data read
size_t au_socket_tcp_recv(AU_Socket socket, void* message, size_t buffer_length);
//Send a message with a given length over UDP
void au_socket_udp_send(AU_Socket socket, const void* message, size_t length);
//Receive data from UDP and return the amount of data read
//TODO: Add a user-facing way to determine the origin of the message
size_t au_socket_udp_recv(AU_Socket socket, void* message, size_t buffer_length);
//Close a socket and shut down its IO
void au_socket_close(AU_Socket socket);
