//
// Created by brick on 12/10/2025.
//

#include "../include/cnet.h"


#if defined(_WIN32)

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

bool CNet_socketShutdown(CNet_socket_instance *socket) {
    if(socket->socket != - 1) {
        shutdown(socket->socket, SD_SEND);
        closesocket(socket->socket);
        socket->socket = -1;
        socket->socketType = CNET_SOCKET_UNINIT_TYPE;
        return true;
    }
    return false;
}

bool CNet_socketSend(CNet_socket_instance *socket, char *buffer) {

    if (socket->socketType != CNET_SOCKET_CLIENT_TYPE && socket->socketType != CNET_SOCKET_SERVER_CONNECTION_TYPE) {
        socket->errorCode = CNET_SOCKET_INCORRECT_TYPE_ERROR;
        return false;
    }


    int returnResult = 0;
    returnResult = send(socket->socket, buffer, (int) strlen(buffer), 0);
    if (returnResult == SOCKET_ERROR) {
        socket->errorCode = CNET_SOCKET_WRITE_ERROR;
        return false;
    }
    return true;

}

bool CNet_socketRecv(CNet_socket_instance *socket, char *buffer, int32_t bufferSize) {

    if (socket->socketType != CNET_SOCKET_CLIENT_TYPE && socket->socketType != CNET_SOCKET_SERVER_CONNECTION_TYPE) {
        socket->errorCode = CNET_SOCKET_INCORRECT_TYPE_ERROR;
        return false;
    }

    memset(buffer, 0, 512);

    if (socket->socket == INVALID_SOCKET) {
        return false;
    }
    int returnResult = 0;
    returnResult = recv(socket->socket, buffer, bufferSize, 0);
    if (returnResult > 0) {
        return true;
    } else {
        socket->errorCode = CNET_SOCKET_READ_ERROR;
        return false;
    }
}

bool CNet_socketConnect(CNet_socket_instance *clientSocket, CNet_serverStructure request) {

    if (clientSocket->socketType != CNET_SOCKET_CLIENT_TYPE) {
        clientSocket->errorCode = CNET_SOCKET_INCORRECT_TYPE_ERROR;
        return false;
    }

    int returnResult = 0;
    struct addrinfo *result = NULL,
            hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    returnResult = getaddrinfo(request.addr, request.port, &hints, &result);
    if (returnResult != 0) {

        return false;
    }

    clientSocket->socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (clientSocket->socket == INVALID_SOCKET) {
        clientSocket->errorCode = CNET_SOCKET_CREATION_ERROR;
        freeaddrinfo(result);

        return false;
    }

    returnResult = connect(clientSocket->socket, result->ai_addr, (int) result->ai_addrlen);
    if (returnResult == SOCKET_ERROR) {
        clientSocket->errorCode = CNET_SOCKET_CONNECT_ERROR;
        closesocket(clientSocket->socket);
        freeaddrinfo(result);

        return false;
    }

    return true;
}

bool CNet_socketAccept(CNet_socket_instance *serverSocket, CNet_socket_instance *connectionSocket) {

    if (serverSocket->socketType != CNET_SOCKET_SERVER_TYPE) {
        serverSocket->errorCode = CNET_SOCKET_INCORRECT_TYPE_ERROR;
        return false;
    }

    int returnResult;

    returnResult = listen(serverSocket->socket, SOMAXCONN);

    if (returnResult == SOCKET_ERROR) {
        serverSocket->errorCode = CNET_SOCKET_LISTEN_ERROR;
        closesocket(serverSocket->socket);
        return false;
    }

    connectionSocket->socket = accept(serverSocket->socket, NULL, NULL);
    if (connectionSocket->socket == INVALID_SOCKET) {
        serverSocket->errorCode = CNET_SOCKET_ACCEPT_ERROR;
        return false;
    }

    return true;
}

bool CNet_socketHost(CNet_socket_instance *serverSocket, const char *port) {

    if (serverSocket->socketType != CNET_SOCKET_SERVER_TYPE) {
        serverSocket->errorCode = CNET_SOCKET_INCORRECT_TYPE_ERROR;
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
        serverSocket->errorCode = CNET_SOCKET_CREATION_ERROR;
        freeaddrinfo(result);
        return false;
    }

    returnResult = bind(serverSocket->socket, result->ai_addr, (int) result->ai_addrlen);
    if (returnResult == SOCKET_ERROR) {
        serverSocket->errorCode = CNET_SOCKET_BIND_ERROR;
        freeaddrinfo(result);
        return false;
    }
    return true;
}

#elif defined(__linux__)
bool CNet_quit() {
    return true;
}
bool CNet_init() {
    return true;
}

bool CNet_socketShutdown(CNet_socket_instance *socket) {
    close(socket->socket);
    return true;
}

bool CNet_socketSend(CNet_socket_instance *socket, char *buffer) {

    if (socket->socketType != CNET_SOCKET_CLIENT_TYPE && socket->socketType != CNET_SOCKET_SERVER_CONNECTION_TYPE) {
        socket->errorCode = CNET_SOCKET_INCORRECT_TYPE_ERROR;
        return false;
    }

    int returnResult = 0;

    returnResult = send(socket->socket, buffer, (int) strlen(buffer), 0);
    if (returnResult == -1) {
        socket->errorCode = CNET_SOCKET_WRITE_ERROR;
        return false;
    }

    return true;
}

bool CNet_socketRecv(CNet_socket_instance *socket, char *buffer) {

    if (socket->socketType != CNET_SOCKET_CLIENT_TYPE && socket->socketType != CNET_SOCKET_SERVER_CONNECTION_TYPE) {
        socket->errorCode = CNET_SOCKET_INCORRECT_TYPE_ERROR;
        return false;
    }

    memset(buffer, 0, 512);

    int returnResult = 0;

    returnResult = recv(socket->socket, buffer, 512, 0);
    if (returnResult > 0) {
        return true;
    } else {
        socket->errorCode = CNET_SOCKET_READ_ERROR;
        return false;
    }
}

bool CNet_socketConnect(CNet_socket_instance *clientSocket, CNet_serverStructure request) {

    if (clientSocket->socketType != CNET_SOCKET_CLIENT_TYPE) {
        clientSocket->errorCode = CNET_SOCKET_INCORRECT_TYPE_ERROR;
        return false;
    }

    struct sockaddr_in serv_addr;
    struct hostent *server;

    server = gethostbyname(request.addr);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    clientSocket->socket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket->socket < 0) {
        clientSocket->errorCode = CNET_SOCKET_CREATION_ERROR;
        return false;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(atoi(request.port));

    int returnResult = 0;

    returnResult = connect(clientSocket->socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (returnResult < 0) {
        clientSocket->errorCode = CNET_SOCKET_CONNECT_ERROR;

        return false;
    }

    return true;
}

bool CNet_socketAccept(CNet_socket_instance *serverSocket, CNet_socket_instance *connectionSocket) {
    socklen_t clilen;
    struct sockaddr_in cli_addr;

    if (serverSocket->socketType != CNET_SOCKET_SERVER_TYPE) {
        serverSocket->errorCode = CNET_SOCKET_INCORRECT_TYPE_ERROR;
        return false;
    }

    int returnResult;

    listen(serverSocket->socket, 5);
    clilen = sizeof(cli_addr);


    connectionSocket->socket = accept(serverSocket->socket, (struct sockaddr *) &cli_addr, &clilen);

    if (connectionSocket->socket < 0) {
        serverSocket->errorCode = CNET_SOCKET_ACCEPT_ERROR;
        return false;
    }

    return true;
}

bool CNet_socketHost(CNet_socket_instance *serverSocket, const char *port) {

    if (serverSocket->socketType != CNET_SOCKET_SERVER_TYPE) {
        serverSocket->errorCode = CNET_SOCKET_INCORRECT_TYPE_ERROR;
        return false;
    }

    struct sockaddr_in serv_addr;

    int returnResult;

    serverSocket->socket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket->socket < 0) {
        serverSocket->errorCode = CNET_SOCKET_CREATION_ERROR;
        return false;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    serv_addr.sin_port = htons(atoi(port));

    returnResult = bind(serverSocket->socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (returnResult < 0) {
        serverSocket->errorCode = CNET_SOCKET_BIND_ERROR;
        return false;
    }
    return true;
}
#endif

bool CNet_socketClose(CNet_socket_instance ** socket) {
    CNet_socketShutdown(*socket);
    CNet_socketDestroy(socket);
    return true;
}

const char *CNet_getError(enum CNET_ERROR_CODES errorCode) {
    const char *message = "";
    switch (errorCode) {
        case CNET_SOCKET_INIT_ERROR:
            message = "Unable to initialize socket";
            break;
        case CNET_SOCKET_WRITE_ERROR:
            message = "Unable to write to socket";
            break;
        case CNET_SOCKET_CREATION_ERROR:
            message = "Unable to create socket";
            break;
        case CNET_SOCKET_READ_ERROR:
            message = "Unable to read from socket";
            break;
        case CNET_SOCKET_BIND_ERROR:
            message = "Unable to bind to socket";
            break;
        case CNET_SOCKET_LISTEN_ERROR:
            message = "Unable to listen to socket";
            break;
        case CNET_SOCKET_ALLOCATE_ERROR:
            message = "Unable to allocate socket";
            break;
        case CNET_SOCKET_CONNECT_ERROR:
            message = "Unable to connect to server via socket";
            break;
        case CNET_SOCKET_ACCEPT_ERROR:
            message = "Unable to accept new connection from socket";
            break;
        case CNET_SOCKET_INCORRECT_TYPE_ERROR:
            message = "Incorrect initialization socket type for requested function";
            break;
    }
    return message;
}

bool CNet_socketDestroy(CNet_socket_instance **socket) {
    if (!socket) {
        return false;
    } else {
        free(*socket);
        return true;
    }
}

bool CNet_socketInit(CNet_socket_instance **socket, enum CNET_SOCKET_TYPES sockType) {
    *socket = malloc(sizeof(CNet_socket));
    (*socket)->socketType = sockType;
    (*socket)->socket = -1;
    return true;
}