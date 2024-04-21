/*
 * TCPDataClient.cpp
 *
 *  Created on: 03.03.2024
 *      Author: rako
 */

#include "TCPDataClient.h"

TCPDataClient::TCPDataClient()
{
	// TODO Auto-generated constructor stub
	init();

}

TCPDataClient::~TCPDataClient()
{
	// TODO Auto-generated destructor stub
	/*
	if ( m_connectThread != nullptr )
	{
		if ( m_connectThread->joinable() )
		{
			m_connectThread->join();
		}
		delete m_connectThread;
	}
	*/

	if ( m_connectThread.joinable() )
	{
		m_connectThread.join();
	}

	if (m_ClientSock != nullptr)
	{
		delete m_ClientSock;
		m_ClientSock = nullptr;
	}
}

void TCPDataClient::init()
{
   printf("TCPDataClient:: init()\n");
   m_ipAddress = getIpAddress();
   printf("IPAddress = %s\n", m_ipAddress.c_str());

//   if (checkValidIPv4())
   /*
   {
      if (m_connectThread == nullptr )
      {
	      m_connectThread = new std::thread(&TCPDataClient::doCreateConnect, this);
      }
   }
   */
   m_connectThread = std::thread(&TCPDataClient::doCreateConnect, this);

/*
   else
   {
	   printf("TCPDataClient no valid IPAddress found --> check file %s\n", FILEPATH);
   }
   */
}

std::string TCPDataClient::getIpAddress()
{
	std::string str_IP     = "";
	std::string str_line   = "";
	std::string str_search = "IPAddress::";

	std::ifstream file;
	file.open(FILEPATH);

	if(!file.is_open())
	{
	   printf("Can't open file --> RETURN %s\n", FILEPATH);
	   return nullptr;
	}
	else
	{
	   printf("Open file %s\n", FILEPATH);
	   file.seekg(std::ios::beg);  // Set ptr to the file beginning
	   while(getline(file, str_line))
	   {
		   size_t found = str_line.find(str_search);
		   printf("found:: %d\n", found);
		   if (found >= 0)
		   {
		     str_IP = str_line.substr(found + str_search.length());
		     break;
		   }
	   }
	}
	file.close();
	return str_IP;;
}

bool TCPDataClient::checkValidIPv4()
{
	 unsigned char a,b,c,d;
	 char buf[25];
	 strcpy( buf, m_ipAddress.c_str());
	 buf[m_ipAddress.length()] = '\0';
	 return sscanf(buf,"%d.%d.%d.%d", &a, &b, &c, &d) == 4;
}

void TCPDataClient::doCreateConnect()
{
	int connectFailureCount = 0;
	BYTE sndBuffer[BUFFSIZE+1];
	BYTE rcvBuffer[BUFFSIZE+1];
	std::string str_sndCommand = "get 16.7.0";   //send Command to receive the actual current value
	strcpy( (char*)sndBuffer, str_sndCommand.c_str());
	sndBuffer[str_sndCommand.length()] = '\0';
	int len = strlen((char*)sndBuffer);
	while(true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(6000));
		printf("new clientSock\n");
		m_ClientSock = new CSock();
		if (m_ClientSock->create(TCP_PORT, m_ipAddress))
		{
		   printf("Send command %s\n", sndBuffer);
		   m_ClientSock->doSend(sndBuffer, len);
		   while(true)
		   {
		   		std::this_thread::sleep_for(std::chrono::milliseconds(25));
		   		int bytesRecv = m_ClientSock->doReceive(rcvBuffer, BUFFSIZE);
		   		printf("Bytes receive: %d\n", bytesRecv);
		   		if (bytesRecv > 0)
		   		{
		   			rcvBuffer[bytesRecv] = '\0';
		   			m_actualCurrent = atoi((char*)rcvBuffer);
		   			printf("actual current value::%d\n", m_actualCurrent);
		   			connectFailureCount = 0;
		   			m_valueChanged = true;
		   			if ( m_ClientSock != nullptr)
		   			{
		   			   printf("delete clientSock1\n");
		   			   delete m_ClientSock;
		   			   m_ClientSock = nullptr;
		   			}
		   			break;
		   		}
		   		else
		   		{
		   			break;
		   		}
		   	}
		}
		else
		{
			connectFailureCount++;
			printf("Connect failure count %d\n", connectFailureCount);
		}
		if ( m_ClientSock != nullptr)
		{
		   printf("delete clientSock2\n");
		   delete m_ClientSock;
		   m_ClientSock = nullptr;
		}
	}
}



