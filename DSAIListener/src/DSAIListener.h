#ifndef DSAILISTENER_H
#define DSAILISTENER_H
#include <iostream>
#include <string>
#include <string.h>
#include <functional>
#include <sys/types.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <array>
#include "utils/BasicTimer.h"


using SOCKET = int;
#define MAX_BUFFER_SIZE  49152
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define MAX_CLIENTS 30

// Forward declaration of class
class DSAIListener;

// Callback to data received
using RecivedMessageHandler = std::function<void(DSAIListener* listener, int nSocketId, const std::string strMsg)>;


class DSAIListener
{
public:
    // Constructor
    DSAIListener(std::string strIPAddress, int m_nPort, const RecivedMessageHandler& handler);

    // Destructor
    ~DSAIListener();

    // Send a message to the specified client
    void Send(int nClientSocket, const std::string& strMsg);

    // Initialize winsock
    bool Init();

    // The server processing logic
    void Run();

    // Clean up after using the service
    void Cleanup();

private:

    // Create a socket
    SOCKET CreateListeningSocket();

    // Handle new client connection
    SOCKET HandleNewConnection(SOCKET listening);

    // Console log information about a socket
    void LogSocketInfo(const std::string& strSockName, SOCKET sockID, const sockaddr_in& sockInfo);

    //Prepare master fd set
    void PrepareFDMasterSet();

    // Wait for new client connections
    void WaitForConnection();

    // Handle already established client connections
    void HandleActiveConnections();

    // Notify all connected clients periodically
    void NotifyClients();

    // Buffer for client messages
    char m_msgBuff[MAX_BUFFER_SIZE];

    // Keep track of the listening socket
    SOCKET m_listeningSock;

    // Keep track of the clients
    std::array<int, MAX_CLIENTS> m_arrClients;

    // current highest socket
    SOCKET m_sockMax;

    // Master socket set, managing input and output connections
    fd_set m_fdMasterSet;

    // Address of the server
    std::string				m_strIPAddress;

    // Listening port
    int						m_nPort;

    // Message received event handler
    RecivedMessageHandler	m_messageHandler;

    //Timer fields
    TimerID m_timerID;
    int m_nTimerPeriod;
    BasicTimer m_timer;
};

#endif // DSAILISTENER_H
