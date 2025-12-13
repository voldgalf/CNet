#include <stdio.h>

#include "include/cnet.h"

int main(void) {

    CNet_init();

    CNet_socket_object * myServer = NULL;
    if(!CNet_socketInit(&myServer, CNET_SOCKET_SERVER_TYPE)) {
        printf("Error: %s\n", CNet_getError(myServer->errorCode));
    }

    CNet_socket_object * myConnection = NULL;
    if(!CNet_socketInit(&myConnection, CNET_SOCKET_SERVER_CONNECTION_TYPE)) {
        printf("Error: %s\n", CNet_getError(myServer->errorCode));
    }

    if(!CNet_socketHost(myServer, "1234")) {
        printf("Error: %s\n", CNet_getError(myServer->errorCode));
    }

    printf("Waiting for connection...\n");

    char buffer[512];
    if(!CNet_socketAccept(myServer, myConnection)) {
        printf("Error: %s\n", CNet_getError(myServer->errorCode));
    }

    printf("Connection found!\n");

    while(true) {

        if (!CNet_socketRecv(myConnection, buffer)) {
            printf("Error: %s\n", CNet_getError(myServer->errorCode));
            break;
        }

        printf("Received: %s\n", buffer);

        if(!CNet_socketSend(myConnection, buffer)) {
            printf("Error: %s\n", CNet_getError(myServer->errorCode));
            break;
        }
    }
    CNet_socketShutdown(myConnection);
    CNet_socketShutdown(myServer);

    CNet_socketDestroy(&myServer);
    CNet_socketDestroy(&myConnection);

    /*
    CNet_socket_object *clientSocket = NULL;

    if (!CNet_socketInit(&clientSocket, CNET_SOCKET_CLIENT_TYPE)) {
        printf("Error: %s\n", CNet_getError(clientSocket->errorCode));
    }

    if (!CNet_socketConnect(clientSocket, "127.0.0.1", "1234")) {
        printf("Error: %s\n", CNet_getError(clientSocket->errorCode));
    }

    if (!CNet_socketSend(clientSocket, "Hello C")) {
        printf("Error: %s\n", CNet_getError(clientSocket->errorCode));
    }

    CNet_socketShutdown(clientSocket);
    CNet_socketDestroy(&clientSocket);
    */
    CNet_quit();

    return 0;
}
