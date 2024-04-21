/*
 * main.cpp
 *
 *  Created on: 03.03.2024
 *      Author: rako
 */

#include <stdio.h>
#include <cstdlib>
#include <string>
#include <chrono>
#include <thread>

#include "TCPDataClient.h"
#include "LoadControlCalculatur.h"

std::string version("1.0.4");


int main(int argc, char *argv[])
{
   printf("VehicleLoadController %s is starting \n" ,version.c_str());

   TCPDataClient *client = new TCPDataClient();

   LoadControlCalculatur *loadCalc = new LoadControlCalculatur(client);

   while(true)
   {
	   std::this_thread::sleep_for(std::chrono::milliseconds(1000));

   }

   printf("VehicleLoadController ends !!\n");

   if (client != nullptr)
   {
	   delete client;
   }

}



