#ifndef DSAILISTENER_H
#define DSAILISTENER_H
#include <string>
#include <functional>

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
    int CreateListeningSocket();

    // Wait for a connection
    int WaitForConnection(int listening);

    // Address of the server
    std::string				m_strIPAddress;

    // Listening port
    int						m_nPort;

    // Message received event handler
    RecivedMessageHandler	m_messageHandler;
};

#endif // DSAILISTENER_H
