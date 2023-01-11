
#define WIN32_LEAN_AND_MEAN
#include "TCP_Client.h"
#include <winsock2.h>
#include "Ws2tcpip.h"
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h> // per _towchar

// Converte la stringa di caratteri a char* in una stringa di caratteri a wchar_t*
// e la copia in dest, che puo' contenere al massimo dest_size elementi di wchar_t
size_t char_to_wchar(wchar_t* dest, const char* src, size_t dest_size) {
    errno_t err = mbstowcs_s(&dest_size, dest, dest_size, src, _TRUNCATE);
    if (err != 0) {
        // Errore durante la conversione
        return (size_t)-1;
    }
    return dest_size;
}

void start_client(char* ip, int port) {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in server;
    char message[1024], server_reply[2000];
    wchar_t wide_ip[INET_ADDRSTRLEN];

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


    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    // Convertire l'indirizzo IP da testo a binario
   // Utilizzare la funzione char_to_wchar per convertire ip da char* a wchar_t*
    if (char_to_wchar(wide_ip, ip, INET_ADDRSTRLEN) == (size_t)-1) {
        puts("Error converting IP address");
        return;
    }


    // Convertire l'indirizzo IP da testo a binario
    if (InetPton(AF_INET, wide_ip, &server.sin_addr) != 1) {
        puts("Invalid IP address");
        return;
    }

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
