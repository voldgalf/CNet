#include <stdio.h>

#include "include/cnet.h"

int main(void) {

    CNet_init();

    CNet_socket_instance * myServer = NULL;
    if(!CNet_socketInit(&myServer, CNET_SOCKET_SERVER_TYPE)) {
        printf("Error: %s\n", CNet_getError(myServer->errorCode));
    }

    CNet_socket_instance * myConnection = NULL;
    if(!CNet_socketInit(&myConnection, CNET_SOCKET_SERVER_CONNECTION_TYPE)) {
        printf("Error: %s\n", CNet_getError(myServer->errorCode));
    }

    CNet_server_structure serverStructure;
    serverStructure.port = "1234";
    serverStructure.addr = NULL;

    if(!CNet_socketHost(myServer, serverStructure)) {
        printf("Error: %s\n", CNet_getError(myServer->errorCode));
    }

    printf("*----------------------------------*\n");
    printf(" SERVER RUNNING AT 127.0.0.1:1234   \n");
    printf("*----------------------------------*\n");

    printf("Waiting for connection...\n");

    char buffer[512];

    if(!CNet_socketAccept(myServer, myConnection)) {
        printf("Error: %s\n", CNet_getError(myServer->errorCode));
    }

    printf("*-- New Connection --*\n");

    if (!CNet_socketRecv(myConnection, buffer, 512)) {
        printf("Error: %s\n", CNet_getError(myServer->errorCode));
    }

    printf("Received: %s\n", buffer);

    if(!CNet_socketSend(myConnection, buffer, 512)) {
        printf("Error: %s\n", CNet_getError(myServer->errorCode));
    }

    CNet_socketClose(&myServer);
    CNet_socketClose(&myConnection);

    /*
    CNet_socket_instance *clientSocket = NULL;

    if (!CNet_socketInit(&clientSocket, CNET_SOCKET_CLIENT_TYPE)) {
        printf("Error: %s\n", CNet_getError(clientSocket->errorCode));
    }

    CNet_server_structure serverStructure;
    serverStructure.addr = "127.0.0.1";
    serverStructure.port = "1234";
    printf("Attempting to connect to server ... ");

    if (!CNet_socketConnect(clientSocket, serverStructure)) {
        printf("ERROR\n\t%s\n", CNet_getError(clientSocket->errorCode));
        return -1;
    }

    printf(" SUCCESS\n");

    char * message = "Hello C";

    printf("Attempting to send message ... ");
    if (!CNet_socketSend(clientSocket, message)) {
        printf("ERROR\n\t%s\n", CNet_getError(clientSocket->errorCode));
        return -1;
    }

    printf(" SUCCESS\n");

    CNet_socketClose(&clientSocket);
    */
    CNet_quit();

    return 0;
}