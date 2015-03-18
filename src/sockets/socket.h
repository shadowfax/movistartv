#ifndef MOVISTARTV_SOCKETS_SOCKET_H
#define MOVISTARTV_SOCKETS_SOCKET_H

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#endif

#include <sys/types.h>

#ifdef WIN32
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#endif

#include <stdlib.h>
#include <stdio.h>

#ifndef WIN32
#define closesocket close
#endif

#ifdef WIN32
// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#endif

#endif