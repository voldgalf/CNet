# CNet "C Networking"

A simple library for simple networking

## Usage

### Server example

```c
// Initialize CNet
CNet_init();

// Create empty pointer to CNet_socket_instance
CNet_socket_instance * myServer = NULL;

// Initialize 'myServer'
CNet_socketInit(&myServer, SOCKET_TYPE_SERVER);

CNet_socket_instance * myConnection = NULL;

CNet_socketInit(&myConnection, SOCKET_TYPE_CLIENT);

// Host
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

CNet_quit();

```
