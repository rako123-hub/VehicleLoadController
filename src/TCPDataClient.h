/*
 * TCPDataClient.h
 *
 *  Created on: 03.03.2024
 *      Author: rako
 */

#ifndef TCPDATACLIENT_H_
#define TCPDATACLIENT_H_

#include <stdio.h>
#include <cstdlib>
#include <string>
#include <fstream>
#include "CSock.h"

#define FILEPATH "/usr/local/bin/VehicleLoadController/config.ini"

class TCPDataClient
{
public:
	TCPDataClient();
	virtual ~TCPDataClient();

public:
	//std::thread *m_connectThread = nullptr;
	std::thread m_connectThread;
	void doCreateConnect();
	int32_t getActualCurrent()         { return m_actualCurrent; }
	void setValueChanged( bool change) { m_valueChanged = change; }
	bool getValueChanged()             { return m_valueChanged; }

private:
	void init();
	std::string getIpAddress();
	bool openTCPConnection();
	bool checkValidIPv4();

	std::string m_ipAddress = "";
	CSock *m_ClientSock = nullptr;
	int32_t m_actualCurrent;
	bool m_valueChanged = false;
};

#endif /* TCPDATACLIENT_H_ */
