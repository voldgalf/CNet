//
// Created by brick on 12/10/2025.
//

#include "../include/globals.h"

const char *CN_get_error(enum CN_errorCodes errorCode) {
const char *message;
switch (errorCode) {
    case NP_NO_ERROR:
        message = "Theres no errors";
        break;
    case NONEXISTENT_SOCKET:
        message = "Socket does not exist";
        break;
    case SOCKET_CREATE_ERROR:
        message = "Error creating socket";
        break;
    case SOCKET_READ_ERROR:
        message = "Error reading socket";
        break;
    case SOCKET_WRITE_ERROR:
        message = "Error writing to socket";
        break;
    case SERVER_LISTEN_ERROR:
        message = "Error listening to socket";
        break;
    case SERVER_ACCEPT_ERROR:
        message = "Error accepting socket";
        break;
    case SERVER_BIND_ERROR:
        message = "Error binding socket";
        break;
    case INCORRECT_SOCKET_TYPE:
        message = "Incorrect socket type for function";
        break;
}
return message;
}

#ifdef __WIN32

    bool CN_quit() {
        WSACleanup();
        return true;
    }

    bool CN_init() {
        WSADATA wsaData;
        int returnResult = 0;
        returnResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (returnResult != 0) {
            printf("WSAStartup failed with error: %d\n", returnResult);
            return false;
        }
        return true;
    }

    bool CN_socketInit(CN_socket_object ** socket, enum CN_socketTypes sockType) {
        *socket = malloc(sizeof(CN_socket));
        (*socket)->socketType = sockType;
        (*socket)->currentSocket = -1;
        return true;
    }

    bool CN_socketDestroy(CN_socket_object * socket) {
        if(!socket) {
            return false;
        } else {
            free(socket);
            return true;
        }
    }

    bool CN_socketShutdown(CN_socket_object * socket) {
    shutdown(socket->currentSocket, SD_SEND);
    closesocket(socket->currentSocket);
    return true;
    }

    bool CN_socketSend(CN_socket_object* socket, char * buffer) {

    if(!socket) {
        return false;
    }


    int returnResult = 0;
    returnResult = send(socket->currentSocket, buffer, strlen(buffer), 0);
    if (returnResult == SOCKET_ERROR) {
        printf("Error sending data to socket\n");
        return false;
    }
    return true;

    }

    bool CN_socketRecv(CN_socket_object* socket, char * buffer) {

    memset(buffer, 0, 512);

    if (socket->currentSocket == INVALID_SOCKET) {
        return false;
    }
    int returnResult = 0;
    returnResult = recv(socket->currentSocket, buffer, 512, 0);
    if (returnResult > 0) {
        return true;
    }

    return false;
    }

    bool CN_socketConnect(CN_socket_object* server_socket, char * address, char * port) {
    int returnResult = 0;
    struct addrinfo *result = NULL,
            hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    returnResult = getaddrinfo(address, port, &hints, &result);
    if (returnResult != 0) {
        printf("getaddrinfo failed with error: %d\n", returnResult);

        return false;
    }
    server_socket->currentSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (server_socket->currentSocket == INVALID_SOCKET) {
        printf("Unable to create socket!\n");
        freeaddrinfo(result);

        return false;
    }
    returnResult = connect(server_socket->currentSocket, result->ai_addr, (int) result->ai_addrlen);
    if (returnResult == SOCKET_ERROR) {
        printf("Unable to connect to server!\n");
        closesocket(server_socket->currentSocket);
        freeaddrinfo(result);

        return false;
    }

    return true;
}

    bool CN_socketAccept(CN_socket_object* serverSocket, CN_socket_object* connectionSocket) {

    if(!serverSocket || !connectionSocket) {
        return false;
    }

    int returnResult;

    returnResult = listen(serverSocket->currentSocket, SOMAXCONN);

    if (returnResult == SOCKET_ERROR) {
        closesocket(serverSocket->currentSocket);
        return false;
    }

    connectionSocket->currentSocket = accept(serverSocket->currentSocket, NULL, NULL);
    if (connectionSocket->currentSocket == INVALID_SOCKET) {
        return false;
    }
    return true;
    }

    bool CN_socketHost(CN_socket_object * server_socket, char * port) {

    if(!server_socket) {
        return false;
    }

    int returnResult;

    struct addrinfo *result = nullptr;
    struct addrinfo hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    returnResult = getaddrinfo(nullptr, port, &hints, &result);
    if (returnResult != 0) {
        printf("getaddrinfo failed with error: %d\n", returnResult);

        return false;
    }

    server_socket->currentSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (server_socket->currentSocket == INVALID_SOCKET) {
        freeaddrinfo(result);
        return false;
    }

    returnResult = bind(server_socket->currentSocket, result->ai_addr, (int) result->ai_addrlen);
    if (returnResult == SOCKET_ERROR) {
        freeaddrinfo(result);
        return false;
    }
    return true;
    }


#endif
