#ifndef HOST_LIST_MANAGER
#define HOST_LIST_MANAGER

#include <string>
#include <vector>
#include <iostream>
#include <pthread.h>
#include <time.h>
#include "QtIncludes.h"

class HostListManager : public QObject
{

public:
    HostListManager(QObject* parent);
    void setHostString(QListWidgetItem* selectedItem);
    inline std::string getHostString() {return currentHostString;}
    void connectToServer();
    void checkServerMessages();

    virtual ~HostListManager() {}
public slots:
    void updateList(const QString& serverMessage);
public:
    inline void setTextEditRef(QTextEdit* textEdit) {textEditRef = textEdit;}
    inline void setTextBrowserRef(QTextBrowser* textBrowser) {textBrowserRef = textBrowser;}
private:
    std::string currentHostString;
    int serverFd = -1;
    QTextEdit* textEditRef = nullptr;
    QTextBrowser* textBrowserRef = nullptr;
    pthread_t checkMessageThreadId;
    pthread_mutex_t checkMessageMutexId;
    std::vector<std::string> serverMessages;
    void getMessagesFromServer();
    bool isConnected = false;

    static void* threadServerBufferCheck(void* hlmRef);
   
};

#endif