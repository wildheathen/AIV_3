#ifndef CLIENT_H
#define CLIENT_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>
#include "Ws2tcpip.h"
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h> // per _towchar
#include "sha256.h"
#include "bomberman.h"

size_t char_to_wchar(wchar_t* dest, const char* src, size_t dest_size);
void start_client(char* ip, int port, player_t* player);

#endif