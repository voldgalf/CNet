//
// Created by brick on 12/10/2025.
//

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef  __WIN32

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#elif __linux__
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <string.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netdb.h>
#endif

typedef unsigned long long CNet_socket;

enum CNET_ERROR_CODES {

    CNET_SOCKET_INIT_ERROR,
    CNET_SOCKET_WRITE_ERROR,
    CNET_SOCKET_CREATION_ERROR,
    CNET_SOCKET_READ_ERROR,
    CNET_SOCKET_BIND_ERROR,
    CNET_SOCKET_LISTEN_ERROR,
    CNET_SOCKET_ALLOCATE_ERROR,
    CNET_SOCKET_CONNECT_ERROR,
    CNET_SOCKET_ACCEPT_ERROR,
    CNET_SOCKET_INCORRECT_TYPE_ERROR,

};

enum CNET_SOCKET_TYPES {
    CNET_SOCKET_SERVER_TYPE,
    CNET_SOCKET_SERVER_CONNECTION_TYPE,
    CNET_SOCKET_CLIENT_TYPE,
};

/*!
* \struct CNet_socket_instance
* \brief data structure used throughout this library
*
* Data structure used for the CNet socket protocol
*/
typedef struct {
    /*!
    * \brief Contains the specified socket type upon initialization
    */
    enum CNET_SOCKET_TYPES socketType;

    /*!
    * \brief The network socket itself
    */
    CNet_socket socket;

    /*!
    * \brief Boolean for whether the socket is still alive
    */
    bool isActive;

    /*!
    * \brief Stored enum of `errorCodes` used in np_get_error()
    */
    enum CNET_ERROR_CODES errorCode;

} CNet_socket_instance;

typedef struct {
    const char * addr;
    const char * port;
} CNet_server;

const char *CNet_getError(enum CNET_ERROR_CODES errorCode);

/*!
* \brief Destroys socket
*
* Destroys socket
*
* @param[in,out] socket an instance of `CNet_socket_instance`
*
* @retval true `socket` is destroyed correctly
* @retval false An error has occurred
*/
bool CNet_socketDestroy(CNet_socket_instance **socket);

/*!
* \brief Quit CNet
*
* Deinitialize platform dependent binaries (ex. winsocks for windows)
*
*
* @retval true CNet is deinitialized correctly
* @retval false An error has occurred
*/
bool CNet_quit();

/*!
* \brief Initialize CNet
*
* Initializes platform dependent binaries (ex. winsocks for windows)
*
*
* @retval true CNet is initialized correctly
* @retval false An error has occurred
*/
bool CNet_init();

/*!
* \brief Initialize socket
*
* Allocate and initialize `socket`
*
* @param[in,out] socket an instance of `CNet_socket_instance`
* @param[in] sockType specified `CNet_socketTypes`
 *
* @retval true socket is allocated and updated
* @retval false An error has occurred
*/
bool CNet_socketInit(CNet_socket_instance **socket, enum CNET_SOCKET_TYPES sockType);

/*!
* \brief Shuts down socket
*
* Shuts down socket
*
* @param[in,out] socket an instance of `CNet_socket_instance`
*
* @retval true `socket` is shutdown correctly
* @retval false An error has occurred
*/
bool CNet_socketShutdown(CNet_socket_instance *socket);

/*!
* \brief Write to socket
*
* Writes to `socket` given `buffer`
*
* @param[in] socket an instance of `CNet_socket_instance`
* @param[in] buffer a char pointer
 *
* @retval true Bytes from `buffer` are written to `socket`
* @retval false An error has occurred
*/
bool CNet_socketSend(CNet_socket_instance *socket, char *buffer);

/*!
* \brief Read from socket
*
* Reads from socket given `socket` and writes bytes to `buffer`
*
* @param[in] socket an instance of `CNet_socket_instance`
* @param[in,out] buffer a char pointer
 *
* @retval true Read bytes from `socket` are written to `buffer`
* @retval false An error has occurred
*/
bool CNet_socketRecv(CNet_socket_instance *socket, char *buffer);

/*!
* \brief Connect to a specified server
*
* Connect to server using `address` and `port` and writes that socket to `socket`
*
* @param[in,out] socket an instance of `CNet_socket_instance`
* @param[in] request an instance of `CNet_server`
*
* @retval true `socket` is now connected to server
* @retval false An error has occurred
*/
bool CNet_socketConnect(CNet_socket_instance *socket, CNet_server request);

/*!
* \brief Writes newly connected connection to socket
*
* Accepts new socket from `serverSocket` and writes it to `connectionSocket`
*
* @param[in,out] serverSocket an instance of `CNet_socket_instance`
* @param[in,out] connectionSocket an instance of `CNet_socket_instance'
 *
* @retval true `connectionSocket` is now written with new connection from `serverSocket`
* @retval false An error has occurred
*/
bool CNet_socketAccept(CNet_socket_instance *serverSocket, CNet_socket_instance *connectionSocket);

/*!
* \brief Hosts server given socket
*
* Given a `CNet_socket_instance` instance and a port: hosts a server
*
* @param[in,out] socket an instance of `CNet_socket_instance`
* @param[in] port a char pointer
 *
* @retval true Server is now being hosted on `port` through `socket`
* @retval false An error has occurred
*/
bool CNet_socketHost(CNet_socket_instance *serverSocket, const char *port);

#ifdef __cplusplus
}
#endif