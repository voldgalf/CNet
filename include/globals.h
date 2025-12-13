//
// Created by brick on 12/10/2025.
//

#pragma once

#ifdef  __WIN32
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdio.h>
#endif


typedef unsigned long long CN_socket;

enum CN_errorCodes {
NP_NO_ERROR,
NONEXISTENT_SOCKET,

SOCKET_CREATE_ERROR,
SOCKET_READ_ERROR,
SOCKET_WRITE_ERROR,

SERVER_LISTEN_ERROR,
SERVER_ACCEPT_ERROR,
SERVER_BIND_ERROR,

INCORRECT_SOCKET_TYPE,
};

enum CN_socketTypes {
SOCKET_TYPE_UNINIT,
SOCKET_TYPE_SERVER,
SOCKET_TYPE_SERVER_CLIENT,
SOCKET_TYPE_CLIENT,
};


typedef struct  {
    enum CN_socketTypes socketType;

    /*!
    * \brief The member variable where the socket location is stored
    */
    CN_socket currentSocket;

    /*!
    * \brief Boolean for whether the socket is still alive
    */
    bool isDead;

    /*!
    * \brief Stored enum of `errorCodes` used in np_get_error()
    */
    enum CN_errorCodes errorCode;
} CN_socket_object;


const char *CN_get_error(enum CN_errorCodes errorCode);


bool complete_shutdown();


bool CN_quit();


bool CN_init();


bool CN_socketInit(CN_socket_object ** socket, enum CN_socketTypes sockType);

/*!
* \brief Destroys socket
*
* Destroys socket
*
* @param[in,out] socket an instance of `CN_socket_object`
*
* @retval `socket` is destroyed correctly
* @retval false An error has occurred
*/
bool CN_socketDestroy(CN_socket_object * socket);

/*!
* \brief Shuts down socket
*
* Shuts down socket
*
* @param[in,out] socket an instance of `CN_socket_object`
*
* @retval `socket` is shutdown correctly
* @retval false An error has occurred
*/
bool CN_socketShutdown(CN_socket_object * socket);

/*!
* \brief Checks if `currentSocket` is active and working
* \return true - `currentSocket` is usable
* \return false - `currentSocket` is unusable
*/
bool CN_is_socket_active(CN_socket_object* socket);

/*!
* \brief Write to socket
*
* Writes to `socket` given `buffer`
*
* @param[in,out] socket an instance of `CN_socket_object`
* @param[in] buffer a char pointer
 *
* @retval true Bytes from `buffer` are written to `socket`
* @retval false An error has occurred
*/
bool CN_socketSend(CN_socket_object* socket, char * buffer);

/*!
* \brief Read from socket
*
* Reads from socket given `socket` and writes bytes to `buffer`
*
* @param[in,out] socket an instance of `CN_socket_object`
* @param[in] buffer a char pointer
 *
* @retval true Read bytes from `socket` are written to `buffer`
* @retval false An error has occurred
*/
bool CN_socketRecv(CN_socket_object* socket, char * buffer);

/*!
* \brief Connect to a specified server
*
* Connect to server using `address` and `port` and writes that socket to `socket`
*
* @param[in,out] socket an instance of `CN_socket_object`
* @param[in] address a char pointer
* @param[in] port a char pointer
*
* @retval true `socket` is now connected to server
* @retval false An error has occurred
*/
bool CN_socketConnect(CN_socket_object* socket, char * address, char * port);

/*!
* \brief Writes newly connected connection to socket
*
* Accepts new socket from `serverSocket` and writes it to `connectionSocket`
*
* @param[in,out] serverSocket an instance of `CN_socket_object`
* @param[in,out] connectionSocket an instance of `CN_socket_object'
 *
* @retval true `connectionSocket` is now written with new connection from `serverSocket`
* @retval false An error has occurred
*/
bool CN_socketAccept(CN_socket_object* serverSocket, CN_socket_object* connectionSocket);

/*!
* \brief Hosts server given socket
*
* Given a `CN_socket_object` instance and a port: hosts a server
*
* @param[in,out] socket an instance of `CN_socket_object`
* @param[in] port a char pointer
 *
* @retval true Server is now being hosted on `port` through `socket`
* @retval false An error has occurred
*/
bool CN_socketHost(CN_socket_object * socket, char * port);
