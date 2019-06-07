#ifndef DSAILISTENER_H
#define DSAILISTENER_H
#include <iostream>
#include <string>
#include <string.h>
#include <functional>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>


using SOCKET = int;
#define MAX_BUFFER_SIZE  49152
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

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

    // The main processing loop
    void Run();

    // Clean up after using the service
    void Cleanup();

private:

    // Create a socket
    SOCKET CreateListeningSocket();

    // Wait for a connection
    SOCKET WaitForConnection(SOCKET listening);

    // Console log information about a socket
    void LogSocketInfo(const std::string& strSockName, SOCKET sockID, const sockaddr_in& sockInfo);

    // Address of the server
    std::string				m_strIPAddress;

    // Listening port
    int						m_nPort;

    // Message received event handler
    RecivedMessageHandler	m_messageHandler;
};

#endif // DSAILISTENER_H
