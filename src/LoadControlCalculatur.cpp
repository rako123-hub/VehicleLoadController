/*
 * LoadControlCalculatur.cpp
 *
 *  Created on: 03.03.2024
 *      Author: rako
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>


#include "LoadControlCalculatur.h"


LoadControlCalculatur::LoadControlCalculatur()
{
	// TODO Auto-generated constructor stub
	init();

}

LoadControlCalculatur::LoadControlCalculatur(TCPDataClient *pClient)
:pdataClient(pClient)
{
	init();
}

LoadControlCalculatur::~LoadControlCalculatur()
{
	// TODO Auto-generated destructor stub

	if ( m_loadCalcThread.joinable() )
	{
	   m_loadCalcThread.join();
	}

	if (gpio != nullptr)
	{
		delete gpio;
	}
}

void LoadControlCalculatur::init()
{

	if (gpio == nullptr)
	{
		gpio = new C_GPIO();
		if (gpio == nullptr)
		{
			printf("No memory for GPIO --> return\n");
			return;
		}
		gpio->sunxi_setup_gpio(L1, OUTPUT, PUD_OFF);
		gpio->sunxi_setup_gpio(L2, OUTPUT, PUD_OFF);
		gpio->sunxi_setup_gpio(L3, OUTPUT, PUD_OFF);
		gpio->sunxi_setup_gpio(N,  OUTPUT, PUD_OFF);

		doSwitchOFF();
	}

    m_loadCalcThread = std::thread(&LoadControlCalculatur::loadCalculateThread, this);
}



void LoadControlCalculatur::loadCalculateThread()
{
	BYTE dataCount = 0;
	while(true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		if(pdataClient->getValueChanged())
		{
			int32_t currentValue = pdataClient->getActualCurrent();
			pdataClient->setValueChanged(false);
			if (actualCurrentVec.size() < VECTOR_MAX_SIZE)  actualCurrentVec.push_back(currentValue);
			else
			{
				actualCurrentVec[dataCount] = currentValue;
				dataCount++;
				if (dataCount >= VECTOR_MAX_SIZE) dataCount = 0;
			}
            int32_t averageCount = 0;
            int32_t average      = 0;
			for ( int32_t value : actualCurrentVec)
			{
				averageCount += value;
				printf("%d,", value);
			}
			int32_t size = actualCurrentVec.size();
			if ( size > 0 ) average = averageCount / size;
			printf("\n Size::%d , averageCount::%d , average::%d\n", size, averageCount, average);
			if (size >= VECTOR_MAX_SIZE)
			{
				doCalculateRelaySwitch(average);
			}
		}
		else
			continue;
	}
}

void LoadControlCalculatur::doCalculateRelaySwitch(int32_t average)
{
	int32_t averageNorm;
	averageNorm = -average;

	printf("average Normiert :%d", averageNorm);

	if (averageNorm > (ONE_WIRE_MIN + OFFSET))
	{
		printf("  TRY L1 \n");
		doSwitch_L1();
	}
	if (averageNorm < OFFSET)
	{
		printf("  TRY SWITCHOFF \n");
		doSwitchOFF();
	}
	printf("\n--------------------------------\n");

	/*
	if (averageNorm > THREE_WIRE_MIN + OFFSET)
	{
		doSwitch_L1_L2_L3();
	}
	else if (averageNorm > TWO_WIRE_MIN + OFFSET)
	{
		doSwitch_L1_L2();   //do switchoff L3
	}
	else if (averageNorm > ONE_WIRE_MIN + OFFSET)
	{
		doSwitch_L1();    // do switchon N and switchoff L2,L3
	}
	else
	{
		doSwitchOFF();
	}
	*/
}

void LoadControlCalculatur::doSwitch_L1_L2_L3()
{
	if (!firstSwitchL1_L2_L3)
	{
	   gpio->sunxi_output_gpio(N,  HIGH);
	   gpio->sunxi_output_gpio(L1, HIGH);
	   gpio->sunxi_output_gpio(L2, HIGH);
	   gpio->sunxi_output_gpio(L3, HIGH);
	   firstSwitchL1_L2_L3 = true;
	   firstSwitchOFF      = false;
	   firstSwitchL1       = false;
	   firstSwitchL1_L2    = false;
	}
}

void LoadControlCalculatur::doSwitch_L1_L2()
{
	if (!firstSwitchL1_L2)
	{
	   gpio->sunxi_output_gpio(L3, LOW);
	   gpio->sunxi_output_gpio(N,  HIGH);
	   gpio->sunxi_output_gpio(L1, HIGH);
	   gpio->sunxi_output_gpio(L2, HIGH);
	   firstSwitchL1_L2    = true;
	   firstSwitchOFF      = false;
	   firstSwitchL1       = false;
	   firstSwitchL1_L2_L3 = false;
	}
}

void LoadControlCalculatur::doSwitch_L1()
{
	if(!firstSwitchL1)
	{
		gpio->sunxi_output_gpio(L2, LOW);
		gpio->sunxi_output_gpio(L3, LOW);
		gpio->sunxi_output_gpio(N,  HIGH);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		gpio->sunxi_output_gpio(L1, HIGH);
		firstSwitchL1       = true;
		firstSwitchOFF      = false;
		firstSwitchL1_L2    = false;
		firstSwitchL1_L2_L3 = false;
	}
}

void LoadControlCalculatur::doSwitchOFF()
{
	if (!firstSwitchOFF)
	{
		gpio->sunxi_output_gpio(L1, LOW);
		gpio->sunxi_output_gpio(L2, LOW);
		gpio->sunxi_output_gpio(L3, LOW);
		gpio->sunxi_output_gpio(N,  LOW);
		firstSwitchOFF      = true;
		firstSwitchL1       = false;
		firstSwitchL1_L2    = false;
		firstSwitchL1_L2_L3 = false;
	}
}

