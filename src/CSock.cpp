/*
 * Sock.cpp
 *
 *  Created on: 26.02.2024
 *      Author: rako
 */

#include "CSock.h"

CSock::CSock()
{
	printf("Default Constructor of CSock !\n");
	m_Socket = INVALID_SOCKET;
	m_Port	 = 0;
}

CSock::CSock(int sock)
{
	printf("socket Constructor of CSock !\n");
	m_Socket = sock;
	m_Port	 = 0;
}

CSock::~CSock()
{
	printf("Default Destructor of Sock\n");
	if (m_Socket != INVALID_SOCKET)
	{
		close(m_Socket);
		m_Socket = INVALID_SOCKET;
	}
}

bool CSock::create(int port)       // create Server Socket
{
	m_Port = port;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//	while (!chkLocal())  Sleep(8000);
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	m_Socket = socket(AF_INET, SOCK_STREAM, 0);		// create socket
	if (m_Socket == INVALID_SOCKET)
	{
		printf("*** ERROR :: CSock: can't create socket\n");
		return (false);
	}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	struct	sockaddr_in m_SockAddr;

	memset(&m_SockAddr, 0, sizeof(m_SockAddr));	// clear m_SockAddr struct
// set type and port
	m_SockAddr.sin_family = AF_INET;
	m_SockAddr.sin_port   = htons((u_short)m_Port);
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	int lErr = bind(m_Socket, (const struct sockaddr *) &m_SockAddr, sizeof(m_SockAddr));
	if (lErr == SOCKET_ERROR)
	{
		printf("*** ERROR :: CSock: bind failed\n");
		close(m_Socket);
		m_Socket = INVALID_SOCKET;
		return (false);
	}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	return (true);
}

bool CSock::create(int port, std::string strAddress)   // create Client Socket and connect to Server
{
	m_Port = port;
	m_Socket = socket(AF_INET, SOCK_STREAM, 0);		// create socket
	if (m_Socket == INVALID_SOCKET)
	{
		printf("*** ERROR :: CSock: can't create socket\n");
		return (false);
	}
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	struct	sockaddr_in m_SockAddr;

	memset(&m_SockAddr, 0, sizeof(m_SockAddr));	// clear m_SockAddr struct
	// set type and port
	m_SockAddr.sin_family = AF_INET;
	m_SockAddr.sin_port   = htons((u_short)m_Port);
	m_SockAddr.sin_addr.s_addr = inet_addr(strAddress.c_str());

	int lErr = connect(m_Socket, (const struct sockaddr *)&m_SockAddr, sizeof(m_SockAddr));
	if (lErr == SOCKET_ERROR)
	{
	   printf("*** ERROR :: CSock: connect to Server failed\n");
	   close(m_Socket);
	   m_Socket = INVALID_SOCKET;
	   return (false);
    }
	return true;
}

bool CSock::doListen(void)
{
	if (m_Socket == INVALID_SOCKET) return (false);

    printf("CSock :: Listening on port %d\n", m_Port);

	int lErr = listen(m_Socket, SOCKMAXCONN);
	if (lErr == SOCKET_ERROR) {
		printf("*** ERROR :: CSock Listen ERROR !!!!!\n");
		return (false);
	}
	return (true);
}


CSock *CSock::doAccept(void)
{
	int	sock;

	if (m_Socket == INVALID_SOCKET) return (NULL);

	sock = accept(m_Socket, 0, 0);
	if (sock == INVALID_SOCKET) {
		printf("accept: ERROR !\n");
		return (NULL);
	}

	printf("CSock:: DoAccept new CSock()\n");
	CSock *newSock = new CSock(sock);
	return (newSock);
}

bool CSock::doSend(BYTE *buf, int len)
{
	int lErr = send(m_Socket, (char *)buf, len, 0);
	if (lErr == SOCKET_ERROR) {
		return (false);
	}
	return (true);
}

//---------------------------------------------------------------------------

int CSock::doReceive(BYTE *buf, int max)
{
	int  iRead;

	iRead = recv(m_Socket, (char *)buf, max, 0);
    if (iRead == SOCKET_ERROR) {
		return (0);
	}
	return (iRead);
}



