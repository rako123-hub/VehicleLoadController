/*
 * LoadControlCalculatur.h
 *
 *  Created on: 03.03.2024
 *      Author: rako
 */

#ifndef LOADCONTROLCALCULATUR_H_
#define LOADCONTROLCALCULATUR_H_

#include <vector>
#include <thread>
#include "TCPDataClient.h"
#include "C_GPIO.h"


#define VECTOR_MAX_SIZE   60   // 60 * 6sec     -->360 sec
#define ONE_WIRE_MIN    2300   // 10A * 230V    --> 2300W
#define TWO_WIRE_MIN    2760   // 6A * 230V * 2 --> 2760W
#define THREE_WIRE_MIN  4140   // 6A * 230V *3  --> 4140W
#define OFFSET           200   // for the first switch to upper step


class LoadControlCalculatur
{
public:
	LoadControlCalculatur();
	LoadControlCalculatur(TCPDataClient *pClient);
	virtual ~LoadControlCalculatur();

public:
	void loadCalculateThread();
	//std::thread *m_loadCalcThread = nullptr;
	std::thread m_loadCalcThread;

private:
	void init();
	void doCalculateRelaySwitch(int32_t average);
	void doSwitch_L1_L2_L3();
	void doSwitch_L1_L2();
	void doSwitch_L1();
	void doSwitchOFF();

	std::vector<int> actualCurrentVec = {};
	TCPDataClient *pdataClient = nullptr;

	C_GPIO *gpio = nullptr;
	bool firstSwitchL1_L2_L3 = false;
	bool firstSwitchL1_L2    = false;
	bool firstSwitchL1       = false;
	bool firstSwitchOFF      = false;

};

#endif /* LOADCONTROLCALCULATUR_H_ */
