#ifndef DSAIListenerWin32_H
#define DSAIListenerWin32_H
#include <iostream>
#include <string>
#include <functional>
#include <WS2tcpip.h>						// Header file for Winsock functions
#pragma comment(lib, "ws2_32.lib")			// Winsock library file

#define MAX_BUFFER_SIZE  49152

// Forward declaration of class
class DSAIListenerWin32;

// Callback to data received
using RecivedMessageHandler = std::function<void(DSAIListenerWin32* listener, int nSocketId, const std::string strMsg)>;


class DSAIListenerWin32
{
public:
	// Constructor
	DSAIListenerWin32(std::string strIPAddress, int m_nPort, const RecivedMessageHandler& handler);

	// Destructor
	~DSAIListenerWin32();

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
