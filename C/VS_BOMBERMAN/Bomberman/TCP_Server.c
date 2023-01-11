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
    while (1) 
    {

        char buffer[1024];
        // Clear the buffer by filling null, it might have previously received data
        memset(buffer, '\0', sizeof(buffer));

        
        // Impostare la modalità di funzionamento non bloccante per il socket
        u_long nonBlockingMode = 1;
        ioctlsocket(client_sock, FIONBIO, &nonBlockingMode);

        
        // Try to receive some data
        int receive = recv(client_sock, buffer, sizeof(buffer), 0);
        if (receive == SOCKET_ERROR) //-1
        {
            int error = WSAGetLastError();
            if (error == WSAEWOULDBLOCK)
            {
                // Il socket è in modalità non bloccante e non ci sono dati da ricevere
                // wchar_t *s = NULL;
                // FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
                //                NULL, WSAGetLastError(),
                //                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                //                (LPWSTR)&s, 0, NULL);
                // fprintf(stderr, "%S\n", s);
                // LocalFree(s);
            }
            
            
        }
        else if (receive == 0) //è stata chiusa la connessione 
        {
            
        }
        else // Dati ricevuti
        {
            buffer[receive] = '\0';
            printf("Received message from client: %s\n", buffer);

            // Check if the client has sent the "exit" message
            if (strcmp(buffer, "exit\n") == 0) 
            {
                close_server(server_sock, client_sock);
                break;
            }

            // Send a reply to the client
            char server_message[1024] = "ACK";
            send(client_sock, server_message, strlen(server_message), 0);   
        }

        
    }
}

void close_server(SOCKET server_sock, SOCKET client_sock)
{
    //NOTIFICARE IL CLIENT CHE E' STATA CHIUSA LA CONNESSIONE
    closesocket(client_sock);
    closesocket(server_sock);
    WSACleanup();
}
