#include "DSAIListenerWin32.h"
#include <iostream>



DSAIListenerWin32::DSAIListenerWin32( const std::string strIPAddress, int nPort, const RecivedMessageHandler& messageHandler)
    : m_strIPAddress(strIPAddress),
      m_nPort(nPort),
      m_messageHandler(messageHandler)
{
}

DSAIListenerWin32::~DSAIListenerWin32()
{
    Cleanup();
}

// Send a message to the specified client
void DSAIListenerWin32::Send(int nClientSocket, const std::string& strMsg)
{
    send(nClientSocket, strMsg.c_str(), strMsg.size() + 1, 0);
}

// Initialize winsock
bool DSAIListenerWin32::Init()
{
	WSAData data;
	WORD ver = MAKEWORD(2, 2);

	int wsInit = WSAStartup(ver, &data);
	// TODO: Inform caller the error that occured

	return wsInit == 0;
}

// The main processing loop
void DSAIListenerWin32::Run()
{
    char buf[MAX_BUFFER_SIZE];

    while (true)
    {
        // Create a listening socket
        SOCKET listeningSock = CreateListeningSocket();
        if (listeningSock == INVALID_SOCKET)
        {
            break;
        }

        SOCKET clientSock = WaitForConnection(listeningSock);
        if (clientSock != INVALID_SOCKET)
        {
            closesocket(listeningSock);

            int bytesReceived = 0;
            do
            {
                memset(buf, 0, MAX_BUFFER_SIZE);

                bytesReceived = recv(clientSock, buf, MAX_BUFFER_SIZE, 0);
                if (bytesReceived > 0)
                {
                    if (m_messageHandler)
                    {
                        m_messageHandler(this, clientSock, std::string(buf, 0, bytesReceived));
                    }
                }

            } while (bytesReceived > 0);

            closesocket(clientSock);
            std::cout << "Client #" << clientSock << " Disconnected!\n";
        }
    }
}

void DSAIListenerWin32::Cleanup()
{
    std::cout << "Closing Down Server!";
	WSACleanup();
}

// Create a socket
SOCKET DSAIListenerWin32::CreateListeningSocket()
{
    SOCKET listeningSock = socket(AF_INET, SOCK_STREAM, 0);
    if (listeningSock != INVALID_SOCKET)
    {
        sockaddr_in hint;
        hint.sin_family = AF_INET;
        hint.sin_port = htons(m_nPort);
        inet_pton(AF_INET, m_strIPAddress.c_str(), &hint.sin_addr);

        int bindOk = bind(listeningSock, (sockaddr*)&hint, sizeof(hint));
        if (bindOk != SOCKET_ERROR)
        {
            int listenOk = listen(listeningSock, SOMAXCONN);
            if (listenOk == SOCKET_ERROR)
            {
                return -1;
            }
        }
        else
        {
            return -1;
        }
    }

    return listeningSock;
}

// Wait for a connection
SOCKET DSAIListenerWin32::WaitForConnection(SOCKET listeningSock)
{
    // Wait for a connection
    sockaddr_in clientInfo;
    socklen_t clientSize = sizeof(clientInfo);

    SOCKET clientSock = accept(listeningSock, nullptr, nullptr);

    // Output client info to console
    char host[NI_MAXHOST];      // Client's remote name
    char service[NI_MAXSERV];   // Service (i.e. port) the client is connect on

    memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);

    if (getnameinfo((sockaddr*)&clientInfo, clientSize, host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
    {
        std::cout << "Client #" << clientSock << ": "
                  << host << " connected on port " << service << std::endl;
    }
    else
    {
        inet_ntop(AF_INET, &clientInfo.sin_addr, host, NI_MAXHOST);
        std::cout << "Client #" << clientSock << ": " << host
                  << " connected on port " << ntohs(clientInfo.sin_port)
                  << std::endl;
    }

    return clientSock;
}
