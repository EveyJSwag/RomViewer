#include "netUtil.h"
#include "hostListManager.h"
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h> 


HostListManager::HostListManager(QObject* parent) : QObject(parent)
{}

void HostListManager::setHostString(QListWidgetItem* selectedItem)
{
    currentHostString = selectedItem->text().toStdString();
    std::cout << "SELECTED THIS ITEM: " << currentHostString << std::endl;
}

void HostListManager::connectToServer()
{
    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in socketAddr;
    socketAddr.sin_family = AF_INET;
    socketAddr.sin_port   = htons(55556);
    inet_pton(AF_INET, currentHostString.c_str(), &socketAddr.sin_addr);
    if (net_util::c_connectToServer(serverFd, (sockaddr*)&socketAddr) != 0 )
    {
        std::cout << "CANNOT CONNECT" << std::endl;
    }
    else if (!isConnected)
    {
        isConnected = true;
        write(
            serverFd, std::string("hello server").c_str(), 
            std::string("hello server").size());

        textEditRef->setEnabled(true);
        textBrowserRef->setEnabled(true);
        pthread_mutex_init(&checkMessageMutexId, NULL);
        pthread_create(&checkMessageThreadId, NULL, &threadServerBufferCheck, this);
    }
}

void* HostListManager::threadServerBufferCheck(void* hlmRef)
{
    HostListManager* staticHlmRef = (HostListManager*) hlmRef;
    staticHlmRef->getMessagesFromServer();
}


void HostListManager::getMessagesFromServer()
{
    unsigned long startTime = (unsigned long)time(NULL);
    while(true)
    {
        if (time(NULL) - startTime >= 3)
        {
            std::cout << "CHECKING MESSAGEZ" << std::endl;
            startTime = (unsigned long)time(NULL);
            char serverMessageBuffer[1024];
            memset(serverMessageBuffer, 0, 1024);
            pthread_mutex_lock(&checkMessageMutexId);
            read(serverFd, serverMessageBuffer, 1024);

            bool isUnique = true;
            std::string serverMessage=std::string(serverMessageBuffer);
            for (std::vector<std::string>::iterator itr = serverMessages.begin();
                 itr != serverMessages.end(); 
                 itr++)
            {
                if (serverMessage.compare(*itr) == 0)
                {
                    isUnique = false;
                    break;
                }
            }
            if (isUnique)
            {
                std::cout << "PUTTING IN LIST" << std::endl;
                updateList(QString(serverMessage.c_str()));
            }
            std::cout << serverMessageBuffer << std::endl;

            pthread_mutex_unlock(&checkMessageMutexId);
        }   
    }
}

void HostListManager::checkServerMessages()
{

    if (isConnected)
    {
        std::cout << "UPDATING WIDGET" << std::endl;
        //pthread_mutex_lock(&checkMessageMutexId);
        if (serverMessages.size() > 0)
            textBrowserRef->setText(  serverMessages[serverMessages.size() - 1].c_str()  );
        //pthread_mutex_unlock(&checkMessageMutexId);
    }

}

void HostListManager::updateList(const QString& serverMessage)
{
    serverMessages.push_back(serverMessage.toStdString());
}
