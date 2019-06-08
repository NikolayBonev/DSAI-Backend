#include "DSAIListenerWin32.h"

DSAIListenerWin32::DSAIListenerWin32(const std::string strIPAddress, int nPort, const RecivedMessageHandler& messageHandler)
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
	send(nClientSocket, strMsg.c_str(), strMsg.size(), 0);
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
			std::cerr << "Invalid server socket created!\n";
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
		else
		{
			std::cerr << "Invalid client socket created!\n"
				<< strerror(errno) << std::endl;
			closesocket(clientSock);
		}
	}
}

void DSAIListenerWin32::Cleanup()
{
	std::cout << "Closing Down Server!\n";
	WSACleanup();
}

// Create a socket
SOCKET DSAIListenerWin32::CreateListeningSocket()
{
	SOCKET listeningSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Fix Address Already in Use Bug
	std::string enable;
	if (setsockopt(listeningSock, SOL_SOCKET, SO_REUSEADDR, enable.c_str(), sizeof(int)) < 0)
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
SOCKET DSAIListenerWin32::WaitForConnection(SOCKET listeningSock)
{
	// Wait for a connection
	sockaddr_in clientInfo;
	SOCKET clientSock = accept(listeningSock, nullptr, nullptr);
	LogSocketInfo("Client", clientSock, clientInfo);

	return clientSock;
}

void DSAIListenerWin32::LogSocketInfo(const std::string& strSockName, SOCKET sockID, const sockaddr_in& sockInfo)
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
