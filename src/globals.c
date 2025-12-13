//
// Created by brick on 12/10/2025.
//

#include "../include/globals.h"

#ifdef __cplusplus
extern "C" {
#endif

const char * CNet_get_error(enum CNet_errorCodes errorCode) {
    const char * message = "";
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
    case SOCKET_CONNECT_ERROR:
        message = "Error connecting socket to server";
        break;
}
    return message;
}

#ifdef __WIN32

    bool CNet_quit() {
        WSACleanup();
        return true;
    }

    bool CNet_init() {
        WSADATA wsaData;
        int returnResult = 0;
        returnResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (returnResult != 0) {
            printf("WSAStartup failed with error: %d\n", returnResult);
            return false;
        }
        return true;
    }

    bool CNet_socketInit(CNet_socket_object ** socket, enum CNet_socketTypes sockType) {
        *socket = malloc(sizeof(CNet_socket));
        (*socket)->socketType = sockType;
        (*socket)->socket = -1;
        return true;
    }

    bool CNet_socketDestroy(CNet_socket_object ** socket) {
        if(!socket) {
            return false;
        } else {
            free(*socket);
            return true;
        }
    }

    bool CNet_socketShutdown(CNet_socket_object * socket) {
        shutdown(socket->socket, SD_SEND);
        closesocket(socket->socket);
        return true;
    }

    bool CNet_socketSend(CNet_socket_object* socket, char * buffer) {
        if(!socket) {

            return false;
        }

        int returnResult = 0;
        returnResult = send(socket->socket, buffer, strlen(buffer), 0);
        if (returnResult == SOCKET_ERROR){
            socket->errorCode = SOCKET_WRITE_ERROR;
            return false;
        }
        return true;

    }

    bool CNet_socketRecv(CNet_socket_object* socket, char * buffer) {

    memset(buffer, 0, 512);

    if (socket->socket == INVALID_SOCKET) {
        return false;
    }
    int returnResult = 0;
    returnResult = recv(socket->socket, buffer, 512, 0);
    if (returnResult > 0) {
        return true;
    } else {
        socket->errorCode = SOCKET_READ_ERROR;
        return false;
    }
    }

    bool CNet_socketConnect(CNet_socket_object* clientSocket, char * address, char * port) {
        int returnResult = 0;
        struct addrinfo *result = NULL,
                hints;

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        returnResult = getaddrinfo(address, port, &hints, &result);
        if (returnResult != 0) {

            return false;
        }

        clientSocket->socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (clientSocket->socket == INVALID_SOCKET) {
            clientSocket->errorCode = SOCKET_CREATE_ERROR;
            freeaddrinfo(result);

            return false;
        }

        returnResult = connect(clientSocket->socket, result->ai_addr, (int) result->ai_addrlen);
        if (returnResult == SOCKET_ERROR) {
            clientSocket->errorCode = SOCKET_CONNECT_ERROR;
            closesocket(clientSocket->socket);
            freeaddrinfo(result);

            return false;
        }

        return true;
    }

    bool CNet_socketAccept(CNet_socket_object* serverSocket, CNet_socket_object* connectionSocket) {

    if(!serverSocket || !connectionSocket) {
        return false;
    }

    int returnResult;

    returnResult = listen(serverSocket->socket, SOMAXCONN);

    if (returnResult == SOCKET_ERROR) {
        serverSocket->errorCode = SERVER_LISTEN_ERROR;
        closesocket(serverSocket->socket);
        return false;
    }

    connectionSocket->socket = accept(serverSocket->socket, NULL, NULL);
    if (connectionSocket->socket == INVALID_SOCKET) {
        serverSocket->errorCode = SERVER_ACCEPT_ERROR;
        return false;
    }

    return true;
    }

    bool CNet_socketHost(CNet_socket_object * serverSocket, char * port) {

    if(!serverSocket) {
        return false;
    }

    int returnResult;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    returnResult = getaddrinfo(NULL, port, &hints, &result);
    if (returnResult != 0) {

        return false;
    }

    serverSocket->socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (serverSocket->socket == INVALID_SOCKET) {
        serverSocket->errorCode = SOCKET_CREATE_ERROR;
        freeaddrinfo(result);
        return false;
    }

    returnResult = bind(serverSocket->socket, result->ai_addr, (int) result->ai_addrlen);
    if (returnResult == SOCKET_ERROR) {
        serverSocket->errorCode = SERVER_BIND_ERROR;
        freeaddrinfo(result);
        return false;
    }
    return true;
    }


#endif

#ifdef __cplusplus
}
#endif