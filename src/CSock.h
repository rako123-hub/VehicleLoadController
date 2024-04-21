/*
 * Sock.h
 *
 *  Created on: 26.02.2024
 *      Author: rako
 */

#ifndef SRC_CSOCK_H_
#define SRC_CSOCK_H_

#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <thread>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "GlobalDefs.h"

class CSock
{
public:
	CSock	();
	CSock	(int sock);
	virtual ~CSock();

protected:
	int 	m_Socket;
	int		m_Port;

public:
	bool	create		(int port);                          // create ServerSocket
	bool    create      (int port, std::string strAddress);  // create ClientSocket and connect to Server
	bool	doListen	(void);
	CSock	*doAccept	(void);
	bool	doSend		(BYTE *buf, int len);
	int		doReceive	(BYTE *buf, int max);

};

#endif /* SRC_CSOCK_H_ */
