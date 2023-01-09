#ifndef SERVER_H
#define SERVER_H
#include <winsock2.h>

void start_server(int port);
void receive_messages(SOCKET server_sock, SOCKET client_sock);
void close_server(SOCKET server_sock, SOCKET client_sock);

#endif
