#include "TCP_Client.h"
#include "TCP_Server.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <server/client> <port>\n", argv[0]);
        return 1;
    }

    char* mode = argv[1];
    int port = atoi(argv[2]);

    if (strcmp(mode, "server") == 0) {
        start_server(port);
    }
    else if (strcmp(mode, "client") == 0) {
        start_client("127.0.0.1", port);
    }
    else {
        printf("Invalid mode: %s\n", mode);
        return 1;
    }

    return 0;
}
