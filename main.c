#include <stdio.h>
#include <stdbool.h>

#include "include/globals.h"

int main(void) {

    CNet_init();
    /*
    CNet_socket_object * myServer = NULL;

    CNet_socketInit(&myServer, SOCKET_TYPE_SERVER);
    CNet_socket_object * myConnection = NULL;

    CNet_socketInit(&myConnection, SOCKET_TYPE_CLIENT);

    if(!CNet_socketHost(myServer, "1234")) {
        printf("Fail");
    }

    printf("Waiting for connection...\n");

    char buffer[512];
    CNet_socketAccept(myServer, myConnection);

    CNet_socketRecv(myConnection, buffer);

    printf("Received: %s\n", buffer);

    CNet_socketShutdown(myConnection);
    CNet_socketShutdown(myServer);

    CNet_socketDestroy(&myServer);
    CNet_socketDestroy(&myConnection);
     */

    CNet_socket_object * clientSocket = NULL;

    if(!CNet_socketInit(&clientSocket, SOCKET_TYPE_CLIENT)) {
        printf("%s\n", CNet_get_error(clientSocket->errorCode));
    }

    if(!CNet_socketConnect(clientSocket, "127.0.0.1", "1234")) {
        printf("%s\n", CNet_get_error(clientSocket->errorCode));
    }

    if(!CNet_socketSend(clientSocket, "Hello C")) {
        printf("%s\n", CNet_get_error(clientSocket->errorCode));
    }

    printf("Hello, World!\n");

    CNet_quit();

    return 0;
}
