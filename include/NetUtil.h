#ifndef NET_UTIL
#define NET_UTIL
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h> 

namespace net_util
{
int c_connectToServer(int socketFd, struct sockaddr* serverStruct);
};
#endif