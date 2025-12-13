//
// Created by brick on 12/10/2025.
//

#pragma once

#include <stdio.h>
#include <stdbool.h>

#ifdef  __WIN32
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#endif


typedef unsigned long long CNet_socket;

enum CNet_errorCodes {
NP_NO_ERROR,
NONEXISTENT_SOCKET,

SOCKET_CONNECT_ERROR,
SOCKET_CREATE_ERROR,
SOCKET_READ_ERROR,
SOCKET_WRITE_ERROR,

SERVER_LISTEN_ERROR,
SERVER_ACCEPT_ERROR,
SERVER_BIND_ERROR,

INCORRECT_SOCKET_TYPE,
};

enum CNet_socketTypes {
SOCKET_TYPE_UNINIT,
SOCKET_TYPE_SERVER,
SOCKET_TYPE_SERVER_CLIENT,
SOCKET_TYPE_CLIENT,
};

/*!
* \struct CNet_socket_object
* \brief data structure used throughout this library
*
* Data structure used for the CNetet socket protocol
*/
typedef struct  {
    /*!
    * \brief Contains the specified socket type upon initialization
    */
    enum CNet_socketTypes socketType;

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
    enum CNet_errorCodes errorCode;

} CNet_socket_object;


const char * CNet_get_error(enum CNet_errorCodes errorCode);


bool complete_shutdown();


bool CNet_quit();


bool CNet_init();

/*!
* \brief Initialize socket
*
* Allocate and initialize `socket`
*
* @param[in,out] socket an instance of `CNet_socket_object`
* @param[in] sockType specified `CNet_socketTypes`
 *
* @retval true socket is allocated and updated
* @retval false An error has occurred
*/
bool CNet_socketInit(CNet_socket_object ** socket, enum CNet_socketTypes sockType);

/*!
* \brief Destroys socket
*
* Destroys socket
*
* @param[in,out] socket an instance of `CNet_socket_object`
*
* @retval `socket` is destroyed correctly
* @retval false An error has occurred
*/
bool CNet_socketDestroy(CNet_socket_object ** socket);

/*!
* \brief Shuts down socket
*
* Shuts down socket
*
* @param[in,out] socket an instance of `CNet_socket_object`
*
* @retval `socket` is shutdown correctly
* @retval false An error has occurred
*/
bool CNet_socketShutdown(CNet_socket_object * socket);


bool CNet_is_socket_active(CNet_socket_object* socket);

/*!
* \brief Write to socket
*
* Writes to `socket` given `buffer`
*
* @param[in,out] socket an instance of `CNet_socket_object`
* @param[in] buffer a char pointer
 *
* @retval true Bytes from `buffer` are written to `socket`
* @retval false An error has occurred
*/
bool CNet_socketSend(CNet_socket_object* socket, char * buffer);

/*!
* \brief Read from socket
*
* Reads from socket given `socket` and writes bytes to `buffer`
*
* @param[in,out] socket an instance of `CNet_socket_object`
* @param[in] buffer a char pointer
 *
* @retval true Read bytes from `socket` are written to `buffer`
* @retval false An error has occurred
*/
bool CNet_socketRecv(CNet_socket_object* socket, char * buffer);

/*!
* \brief Connect to a specified server
*
* Connect to server using `address` and `port` and writes that socket to `socket`
*
* @param[in,out] socket an instance of `CNet_socket_object`
* @param[in] address a char pointer
* @param[in] port a char pointer
*
* @retval true `socket` is now connected to server
* @retval false An error has occurred
*/
bool CNet_socketConnect(CNet_socket_object* socket, char * address, char * port);

/*!
* \brief Writes newly connected connection to socket
*
* Accepts new socket from `serverSocket` and writes it to `connectionSocket`
*
* @param[in,out] serverSocket an instance of `CNet_socket_object`
* @param[in,out] connectionSocket an instance of `CNet_socket_object'
 *
* @retval true `connectionSocket` is now written with new connection from `serverSocket`
* @retval false An error has occurred
*/
bool CNet_socketAccept(CNet_socket_object* serverSocket, CNet_socket_object* connectionSocket);

/*!
* \brief Hosts server given socket
*
* Given a `CNet_socket_object` instance and a port: hosts a server
*
* @param[in,out] socket an instance of `CNet_socket_object`
* @param[in] port a char pointer
 *
* @retval true Server is now being hosted on `port` through `socket`
* @retval false An error has occurred
*/
bool CNet_socketHost(CNet_socket_object * serverSocket, char * port);
