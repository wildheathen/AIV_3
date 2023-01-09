#include "TCP_Client.h"
#include <winsock2.h>
#include "Ws2tcpip.h"
#include <stdio.h>

void start_client(char* ip, int port) {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;
    char message[1024], server_reply[2000];

    printf("\nInitializing Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return;
    }

    printf("Initialized.\n");

    // Create a socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
        return;
    }

    printf("Socket created.\n");

    InetPton(AF_INET, ip, &server.sin_addr.s_addr);  //inet_addr(ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    // Connect to remote server
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        puts("connect error");
        return;
    }

    puts("Connected");

    // Keep communicating with server
    while (1) {
        printf("Enter message : ");
        fgets(message, 1024, stdin);

        // Send some data
        if (send(sock, message, strlen(message), 0) < 0) {
            puts("Send failed");
            return;
        }

        // Receive a reply from the server
        int recv_size;
        if ((recv_size = recv(sock, server_reply, 2000, 0)) == SOCKET_ERROR) {
            puts("recv failed");
            break;
        }

        server_reply[recv_size] = '\0';
        printf("Server reply : %s\n", server_reply);
    }

    closesocket(sock);
    WSACleanup();
}
