#include "NetUtil.h"
int net_util::c_connectToServer(int socketFd, struct sockaddr* serverStruct)
{
    return connect(socketFd, serverStruct, sizeof(sockaddr));
}