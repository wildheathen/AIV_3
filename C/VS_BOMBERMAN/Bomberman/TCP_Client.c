#include "TCP_Client.h"
#include "Server.h"
#include "bomberman.h"

# define _WINSOCK_DEPRECATED_NO_WARNINGS

int state = 0, random_value;
char message[MAX_PACKET_SIZE];
const char* player_name;
struct sockaddr_in server_address;

void ManagePacket(int rlen);


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

void start_client(char* ip, int port, player_t* player) {
    WSADATA wsa;
    SOCKET sock;
    wchar_t wide_ip[INET_ADDRSTRLEN];
    
    //riferimento al player?
    vec2_t position = {0,0};

    char buffer[MAX_PACKET_SIZE];
    printf( "type w, a, s, or d to move, q to quit\n" );
    int is_running = 1;
    
    #pragma region Winsock_init_and_connection

    printf("\nInitializing Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return;
    }
    printf("Winsock Initialized.\n");
    
    // Create a socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
        return;
    }
    printf("Socket created.\n");

    // Convertire l'indirizzo IP da testo a binario
    // Utilizzare la funzione char_to_wchar per convertire ip da char* a wchar_t*
    if (char_to_wchar(wide_ip, ip, INET_ADDRSTRLEN) == (size_t)-1) {
        puts("Error converting IP address");
        return;
    }

    // Convertire l'indirizzo IP da testo a binario
    if (InetPton(AF_INET, wide_ip, &server_address.sin_addr) != 1) {
        puts("Invalid IP address");
        return;
    }
    
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    //server_address.sin_addr.S_un.S_addr = inet_addr(ip);
    #pragma endregion Winsock_init_and_connection
       
    // Keep communicating with server
    while(is_running)
    {
        scanf_s( "\n%c", &buffer[0], 1 );
    
        // send to server
        int buffer_length = 1;
        int flags = 0;
        SOCKADDR* to = (SOCKADDR*)&server_address;
        int to_length = sizeof(server_address);
        if( sendto( sock, buffer, buffer_length, flags, to, to_length ) == SOCKET_ERROR )
        {
            printf( "sendto failed: %d", WSAGetLastError() );
            return;
        }
    
        // wait for reply
        flags = 0;
        SOCKADDR_IN from;
        int from_size = sizeof( from );
        int bytes_received = recvfrom( sock, buffer, MAX_PACKET_SIZE, flags, (SOCKADDR*)&from, &from_size );
    
        if( bytes_received == SOCKET_ERROR )
        {
            printf( "recvfrom returned SOCKET_ERROR, WSAGetLastError() %d", WSAGetLastError() );
            break;
        }
    
        // grab data from packet
        int read_index = 0;
    
        memcpy( &position.x, &buffer[read_index], sizeof( position.x ) );
        read_index += sizeof( position.x );
    
        memcpy( &position.y, &buffer[read_index], sizeof( position.y ) );
        read_index += sizeof( position.y );
    
        memcpy( &is_running, &buffer[read_index], sizeof( is_running ) );

        //lato client è errato???
        printf( "x:%d, y:%d, is_running:%d\n", position.x, position.y, is_running );
    }
    
    /* ADVANCED CONNECTION
    while (1)
    {
        if(state == 0)
        {
            random_value = rand() % 100000;
            memcpy(message, &random_value, 4);    
            sendto(socket, message, 4, 0, (struct sockaddr*) &server, sizeof(server));

            state = 1;
        }
        else if (state == 1)
        {
            //struct sockaddr_in endPoint;
            socklen_t endPointLength = sizeof(server);
            int rlen = recvfrom(socket, message, 512, 0, (struct sockaddr*) &server, &endPointLength);
            if (rlen == 4)
            {
                int serverValue;
                memcpy(&serverValue, message, 4);
                
                int totalChallenge = random_value + serverValue;
                unsigned char challengeBytes[4];
                memcpy(challengeBytes, &totalChallenge, 4);
                unsigned char hashedChallenge[32];
                memcpy(challengeBytes, 4, hashedChallenge);

                

                

                unsigned char playerNameOriginalBytes[16];
                memcpy(playerNameOriginalBytes, player_name, 16);
                unsigned char playerNameBytes[8];
                memcpy(playerNameBytes, playerNameOriginalBytes, 8);

                unsigned char challengeAndName[32 + 8];
                memcpy(challengeAndName, hashedChallenge, 32);
                memcpy(challengeAndName + 32, playerNameBytes, 8);

                sendto(socket, challengeAndName, 40, 0, (struct sockaddr*) &server, sizeof(server));
                state = 2;
            }
        }
        else if (state == 2)
        {
            // periodically send transform updates
            unsigned int command = 0;
            unsigned int id = 0;
            // float x = transform.position.x;
            // float z = transform.position.z;
            // float yaw = transform.rotation.eulerAngles.y;

            memcpy(message, &command, 4);
            memcpy(message + 4, &id, 4);
            // memcpy(message + 8, &x, 4);
            // memcpy(message + 12, &z, 4);
            // memcpy(message + 16, &yaw, 4);

            sendto(socket, message, 20, 0, (struct sockaddr*) &server, sizeof(server));

            struct sockaddr_in endPoint;
            socklen_t endPointLength = sizeof(endPoint);
            for (int i = 0; i < 500; i++) {
                int rlen = recvfrom(socket, message, 512, 0, (struct sockaddr*) &endPoint, &endPointLength);
                if (rlen >= 4) {
                    ManagePacket(rlen);
                }
            }
        }

/////////////////////////////////////////////

        
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
    */
    
    //closesocket(sock);
    WSACleanup();
}

void ManagePacket(int rlen)
{
    int command = (int)((message[0] << 24) | (message[1] << 16) | (message[2] << 8) | message[3]);
    if (command == 1) {
        if (rlen != 28) {
            /* TODO better to disconnect */
            printf("unexpected message size for command 1");
            return;
        }

        unsigned int playerId = (unsigned int)((message[4] << 24) | (message[5] << 16) | (message[6] << 8) | message[7]);
        float playerX = *((float *)(message + 8));
        float playerZ = *((float *)(message + 12));
        float playerYaw = *((float *)(message + 16));

    /* GameObject newPlayer = GameObject.Instantiate(playerPrefab); */
    /* newPlayer.transform.position = new Vector3(playerX, 1, playerZ); */
    /* newPlayer.transform.rotation = Quaternion.Euler(0, playerYaw, 0); */

    /* string newPlayerName = Encoding.ASCII.GetString(message, 20, 8); */
    /* newPlayer.name = newPlayerName; */

    /* newPlayer.GetComponentInChildren<TextMesh>().text = newPlayerName; */

    /* playersInGame.Add(playerId, newPlayer); */
  } else if (command == 0) {
    if (rlen != 20) {
      /* TODO better to disconnect */
      printf("unexpected message size for command 0");
      return;
    }

      unsigned int playerId = (unsigned int)((message[4] << 24) | (message[5] << 16) | (message[6] << 8) | message[7]);
      float playerX = *((float *)(message + 8));
      float playerZ = *((float *)(message + 12));
      float playerYaw = *((float *)(message + 16));

    /* if (!playersInGame.ContainsKey(playerId)) */
    /* { */
    /*   Debug.Log("unknown player"); */
    /*   return; */
    /* } */

    /* playersInGame[playerId].transform.position = new Vector3(playerX, 1, playerZ); */
    /* playersInGame[playerId].transform.rotation = Quaternion.Euler(0, playerYaw, 0); */
  } else if (command == 2) {
      if (rlen != 8) {
          /* TODO better to disconnect */
          printf("unexpected message size for command 2");
          return;
      }

      unsigned int playerId = (unsigned int)((message[4] << 24) | (message[5] << 16) | (message[6] << 8) | message[7]);

    /* if (!playersInGame.ContainsKey(playerId)) */
    /* { */
    /*   Debug.Log("unknown player"); */
    /*   return; */
    /* } */

    /* Destroy(playersInGame[playerId]); */
    /* playersInGame.Remove(playerId); */
  }
}

