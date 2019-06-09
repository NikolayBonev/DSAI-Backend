#include "DSAIListener.h"

DSAIListener::DSAIListener( const std::string strIPAddress, int nPort, const RecivedMessageHandler& messageHandler)
    : m_bRunning(true),
      m_msgBuff(),
      m_listeningSock(0),
      m_arrClients(),
      m_sockMax(0),
      m_fdMasterSet(),
      m_strIPAddress(strIPAddress),
      m_nPort(nPort),
      m_messageHandler(messageHandler)
{
}

DSAIListener::~DSAIListener()
{
    Cleanup();
}

// Send a message to the specified client
void DSAIListener::Send(int nClientSocket, const std::string& strMsg)
{
    send(nClientSocket, strMsg.c_str(), strMsg.size(), 0);
}

// Initialize DSAI Listener
bool DSAIListener::Init()
{
    //initialise all client_socket[] to 0 so not checked
    for (SOCKET& client: m_arrClients)
    {
        client = 0;
    }

    // Create a listening socket
    m_listeningSock = CreateListeningSocket();
    if (m_listeningSock == INVALID_SOCKET)
    {
        std::cerr << "Invalid server socket created!\n";
        return false;
    }

    // Set all connections to zero
    FD_ZERO(&m_fdMasterSet);

    // Add our first socket that we're interested in interacting with; the listening socket!
    // It's important that this socket is added for our server or else we won't 'hear' incoming
    // connections
    FD_SET(m_listeningSock, &m_fdMasterSet);

    return true;
}

// The main processing loop
void DSAIListener::Run()
{
    while (m_bRunning)
    {
        //clear the socket set
        FD_ZERO(&m_fdMasterSet);

        //add master socket to set
        FD_SET(m_listeningSock, &m_fdMasterSet);
        m_sockMax = m_listeningSock;

        //add client sockets to set
        for (SOCKET& clientSock: m_arrClients)
        {
            //if valid socket descriptor then add to the master FD set
            if(clientSock > 0)
            {
                FD_SET( clientSock , &m_fdMasterSet);
            }

            //highest socket number, need it for the select function
            if(clientSock > m_sockMax)
            {
                m_sockMax = clientSock;
            }
        }


        //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
        int nActivity = select( m_sockMax + 1 , &m_fdMasterSet , nullptr , nullptr , nullptr);

        if ((nActivity < 0))
        {
            std::cerr << "Select error: " << strerror(errno) << std::endl;
        }

        HandleNewConnection();
        HandleActiveConnections();
    }//end while loop


}

void DSAIListener::Cleanup()
{
    std::cout << "Closing Down Server!\n";
    //close listening socket
    close(m_listeningSock);

    /* Remove the listening socket from the master file descriptor set and close it
    * to prevent anyone else trying to connect. */
    FD_CLR(m_listeningSock, &m_fdMasterSet);
}

// Create a socket
SOCKET DSAIListener::CreateListeningSocket()
{
    SOCKET listeningSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Fix Address Already in Use Bug
    int enable = 1;
    if (setsockopt(listeningSock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    {
        std::cerr << "setsockopt(SO_REUSEADDR) failed";
    }

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
            LogSocketInfo("Server", listeningSock, hint);

            if (listenOk == SOCKET_ERROR)
            {
                std::cerr << "An error occured binding the socket!\n"
                          << strerror(errno) << std::endl;
                return -1;
            }
        }
        else
        {
            std::cerr << "An error occured generating the listening socket!\n"
                      << strerror(errno) << std::endl;
            return -1;
        }
    }

    return listeningSock;
}

// Wait for a connection
SOCKET DSAIListener::WaitForConnection(SOCKET listeningSock)
{
    // Wait for a connection
    sockaddr_in clientInfo;
    SOCKET clientSock = accept(listeningSock, nullptr, nullptr);
    std::cout << "New client connected " << listeningSock << std::endl;
    LogSocketInfo("Client", clientSock, clientInfo);

    return clientSock;
}

void DSAIListener::LogSocketInfo(const std::string& strSockName, SOCKET sockID, const sockaddr_in& sockInfo)
{
    // Output socket info to console
    char host[NI_MAXHOST];      // Client's remote name
    char service[NI_MAXSERV];   // Service (i.e. port) the client is connect on
    size_t sockSize = sizeof(sockInfo);

    memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);

    if (getnameinfo((sockaddr*)&sockInfo, sockSize, host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
    {
        std::cout << strSockName << " #" << sockSize << ": "
                  << host << " connected on port " << service << std::endl;
    }
    else
    {
        inet_ntop(AF_INET, &sockInfo.sin_addr, host, NI_MAXHOST);
        std::cout << strSockName << " #" << sockID << ": " << host
                  << " connected on port " << ntohs(sockInfo.sin_port)
                  << std::endl;
    }
}

void DSAIListener::HandleNewConnection()
{
    //If something happened on the master socket ,
    // then its an incoming connection
    if (FD_ISSET(m_listeningSock, &m_fdMasterSet))
    {

        SOCKET clientSock = WaitForConnection(m_listeningSock);
        if (clientSock != INVALID_SOCKET)
        {
            memset(m_msgBuff, 0, MAX_BUFFER_SIZE);
            int bytesReceived = recv(clientSock, m_msgBuff, MAX_BUFFER_SIZE, 0);
            std::cout << "New: bytesReceived -> " << bytesReceived;
            if (bytesReceived > 0 && m_messageHandler)
            {
                m_messageHandler(this, clientSock, std::string(m_msgBuff, 0, bytesReceived));
            }
            else
            {
                close(clientSock);
                std::cout << "Client #" << clientSock << " Disconnected!\n";
            }
        }
        else
        {
            std::cerr << "Invalid client socket created!\n"
                      << strerror(errno) << std::endl;
            close(clientSock);
        }

        //add new socket to array of sockets
        int nSockIndex = 0;
        for (SOCKET& currSock: m_arrClients)
        {
            //if position is empty
            if( currSock == 0 )
            {
                currSock = clientSock;
                std::cout << "Adding to list of sockets as "
                          <<  nSockIndex << std::endl;
                break;
            }
            nSockIndex++;
        }
    }
}

void DSAIListener::HandleActiveConnections()
{
    //else its some IO operation on some other socket :)
    for (SOCKET& clientSock: m_arrClients)
    {
        if (FD_ISSET( clientSock , &m_fdMasterSet))
        {
            memset(m_msgBuff, 0, MAX_BUFFER_SIZE);
            int bytesReceived = recv(clientSock, m_msgBuff, MAX_BUFFER_SIZE, 0);
            std::cout << "Active: bytesReceived -> " << bytesReceived;
            if (bytesReceived > 0 && m_messageHandler)
            {
                m_messageHandler(this, clientSock, std::string(m_msgBuff, 0, bytesReceived));
            }
            else
            {
                // Close the socket and mark as 0 in list for reuse
                close(clientSock);
                clientSock = -1;
                std::cout << "Client #" << clientSock << " Disconnected!\n";
            }
        }
    }
}
