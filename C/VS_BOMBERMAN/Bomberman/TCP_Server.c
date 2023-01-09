#include "TCP_Server.h"
#include <winsock2.h>
#include <stdio.h>

void start_server(int port) {
    WSADATA wsa;
    SOCKET server_sock, client_sock;
    struct sockaddr_in server, client;
    int c;

    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return;
    }

    printf("Initialised.\n");

    // Create a socket
    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
        return;
    }

    printf("Socket created.\n");

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    // Bind
    if (bind(server_sock, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error code : %d", WSAGetLastError());
        return;
    }

    puts("Bind done");

    // Listen to incoming connections
    listen(server_sock, 3);

    // Accept and incoming connection
    puts("Waiting for incoming connections...");

    c = sizeof(struct sockaddr_in);
    while ((client_sock = accept(server_sock, (struct sockaddr*)&client, &c)) != INVALID_SOCKET) {
        puts("Connection accepted");
        receive_messages(server_sock, client_sock);
    }
}

void receive_messages(SOCKET server_sock, SOCKET client_sock) {
    char buffer[1024];

    // Receive data from the client
    int recv_size;
    if ((recv_size = recv(client_sock, buffer, 1024, 0)) == SOCKET_ERROR) {
        puts("recv failed");
        close_server(server_sock, client_sock);
        return;
    }

    // Add a NULL terminating character to make it a proper string before printing
    buffer[recv_size] = '\0';
    printf("Received message from client: %s\n", buffer);
}

void close_server(SOCKET server_sock, SOCKET client_sock) {
    closesocket(client_sock);
    closesocket(server_sock);
    WSACleanup();
}
