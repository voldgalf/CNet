#include <stdio.h>

#include "include/globals.h"

int main(void) {

    CN_init();

    CN_socket_object * myServer = nullptr;

    CN_socketInit(&myServer, SOCKET_TYPE_SERVER);
    CN_socket_object * myConnection = nullptr;

    CN_socketInit(&myConnection, SOCKET_TYPE_CLIENT);

    if(!CN_socketHost(myServer, "1234")) {
        printf("Fail");
    }

    printf("Waiting for connection...\n");

    char buffer[512];
    CN_socketAccept(myServer, myConnection);

    CN_socketRecv(myConnection, buffer);

    printf("Received: %s\n", buffer);

    CN_socketShutdown(myConnection);
    CN_socketShutdown(myServer);

    CN_socketDestroy(myServer);
    CN_socketDestroy(myConnection);
    printf("Hello, World!\n");
    return 0;
}
