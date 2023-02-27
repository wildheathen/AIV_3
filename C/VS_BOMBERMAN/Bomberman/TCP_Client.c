#include "TCP_Client.h"
#include "Server.h"
#include "bomberman.h"

# define _WINSOCK_DEPRECATED_NO_WARNINGS

int state = 0, random_value;
char message[MAX_PACKET_SIZE];
const char* player_name;
struct sockaddr_in server_address;


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

void start_client(char* ip, int port, player_t* player)
{
    bomberman_player_init(player);
    
    WSADATA wsa;
    SOCKET sock;
    wchar_t wide_ip[INET_ADDRSTRLEN];
    
    unsigned short int state = 0;
    
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
        // send the first "handshake"  
        if (state == 0)
        {
            
            // send configuration
            int buffer_length = 1;
            int flags = 0;
            SOCKADDR* to = (SOCKADDR*)&server_address;
            int to_length = sizeof(server_address);
            
            buffer[0] = (unsigned char)Join;
            
            printf("Please choose a name (max 8 characters) ...\n");
            scanf_s( "\n%s", &buffer[1], sizeof(player->name) );
            
            // send to server
            if( sendto( sock, buffer, sizeof((unsigned short)Join)+sizeof(player->name), flags, to, to_length ) == SOCKET_ERROR )
            {
                printf( "sendto failed: %d", WSAGetLastError() );
                return;
            }

            // receive configuration
            flags = 0;
            SOCKADDR_IN from;
            int from_size = sizeof( from ); 
            // wait for reply
            int bytes_received = recvfrom( sock, buffer, MAX_PACKET_SIZE, flags, (SOCKADDR*)&from, &from_size );
            if( bytes_received == SOCKET_ERROR )
            {
                printf( "recvfrom returned SOCKET_ERROR, WSAGetLastError() %d", WSAGetLastError() );
                break;
            }

            if (buffer[0] == '1')
            {
                memcpy(&player->id, &buffer[2], sizeof(player->id));
                printf("Connection accepted from server\nAssigned id is: %hu\n", player->id);
            }
            else
            {
                printf("Connection denied from server");    
            }

            state = 1;
        }
        else if (state == 1) //input
        {
            buffer[0] = (unsigned char)Input;
            memcpy(&buffer[1], &player->id, sizeof(player->id));
            scanf_s( "\n%c", &buffer[3], 1 );


            // send configuration
            int buffer_length = 4;
            int flags = 0;
            SOCKADDR* to = (SOCKADDR*)&server_address;
            int to_length = sizeof(server_address);
            // send to server
            if( sendto( sock, buffer, buffer_length, flags, to, to_length ) == SOCKET_ERROR )
            {
                printf( "sendto failed: %d", WSAGetLastError() );
                return;
            }

            // receive configuration
            flags = 0;
            SOCKADDR_IN from;
            int from_size = sizeof( from ); 
            // wait for reply
            int bytes_received = recvfrom( sock, buffer, MAX_PACKET_SIZE, flags, (SOCKADDR*)&from, &from_size );
            if( bytes_received == SOCKET_ERROR )
            {
                printf( "recvfrom returned SOCKET_ERROR, WSAGetLastError() %d", WSAGetLastError() );
                break;
            }
    
            // grab data from packet
            int read_index = 0;
            unsigned char status;
            
            memcpy( &status, &buffer[read_index], 1 );
            read_index += 1;

            // TODO CONTROLLARE SE L'ID E' DEL CLIENT CORRENTE
            
            read_index = 3;
            
            if (status == '2' )
            {
                memcpy( &player->position.x, &buffer[3], sizeof( player->position.x ) );
                read_index += sizeof( player->position.x );
                
                memcpy( &player->position.y, &buffer[7], sizeof( player->position.y ) );
                read_index += sizeof( player->position.y );
                
                is_running =1;
            
                printf( "x:%f, y:%f\n", player->position.x, player->position.y);    
            }
                
        }
        
    }
    
   // WSACleanup();
}


