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

/*!
* \brief Error code list
*/
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

/*!
* \struct socket_operations
*
* \brief Base class including sending, receiving data and killing sockets
*
*/
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

/*!
* \brief Return string literal given `errorCode`
*/
const char *CN_get_error(enum CN_errorCodes errorCode);

/*!
*
* \brief shutdowns socket protocol
*
*/
bool complete_shutdown();

/*!
* \brief Shutdown and destroys `currentSocket` child
*/
bool CN_quit();

/*!
*
* \brief initializes socket protocol
* \example examples/example_server.cpp
*
*/
bool CN_init();

/*!
*
* \brief initializes socket protocol
* \example examples/example_server.cpp
*
*/
bool CN_socketInit(CN_socket_object ** socket, enum CN_socketTypes sockType);

bool CN_socketDestroy(CN_socket_object * socket);

bool CN_socketShutdown(CN_socket_object * socket);

/*!
* \brief Checks if `currentSocket` is active and working
* \return true - `currentSocket` is usable
* \return false - `currentSocket` is unusable
*/
bool CN_is_socket_active(CN_socket_object* socket);

/*!
* \brief Write to `currentSocket` given string `msg`
* \param [in] msg String given to write to `currentSocket`
* \return true - `msg` is sent to `currentSocket` correctly
* \return false - An error has occurred
*/
bool CN_socketSend(CN_socket_object* socket, char * buffer);

/*!
* \brief Read from `currentSocket` to given `msg`
* \param [in] msg given char pointer to write read bytes to
* \return true - Bytes from `currentSocket` is correctly read and written to `msg`
* \return false - An error has occurred
*/
bool CN_socketRecv(CN_socket_object* socket, char * buffer);

/*!
* \brief Connect to server given `addr` and `port`
* \param [in] addr String given for requested server address
* \param [in] port Int given for requested server port
* \return true - connected to server successfully
* \return false - An error has occurred
*/
bool CN_socketConnect(CN_socket_object* socket, char * address, char * port);

/*!
* \brief Accepts socket from server
* \return \ref classSocket - Accepted connection's socket is returned
*/
bool CN_socketAccept(CN_socket_object* socket, CN_socket_object* connectionSocket);

bool CN_socketHost(CN_socket_object * socket, char * port);
