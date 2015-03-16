/*
	Author: Michael Christen
	Purpose: Interface with Digilent SDK
	Date: 7/20/2014
*/
#include<digilent/waveforms/dwf.h>

#include "main.h"


HDWF * getDevice() {
	int numDevices = 0;
	//Discover devices
	FDwfEnum(enumfilterAll, &numDevices);
	printf("Num devices: %d\n", numDevices);
	if(numDevices < 1) {
		printf("No Device attached\n");
		exit(1);
	}
	DEVID deviceId;
	DEVVER deviceVersion;
	BOOL haveDiscovery = FALSE;
	int devIdx = 0;
	char strVal[32];
	for(int i = 0; i < numDevices; ++i) {
		FDwfEnumDeviceType(i, &deviceId, &deviceVersion);
		printf("ID: %d, VERSION: %d\n", deviceId, deviceVersion);
		FDwfEnumDeviceName(devIdx, strVal);
		printf("Device name: %s\n",strVal);
		if(deviceId == devidDiscovery) {
			haveDiscovery = TRUE;
			devIdx = i;
		}
	}
	if(!haveDiscovery) {
		printf("It's not a discovery, you SUCK!\n");
		exit(1);
	}
	FDwfEnumSN(devIdx, strVal);
	//Rifleman's Creed
	printf("This is my discovery: %s.\nThere are many like it, but this one is mine.\nMy discovery is my best friend. It is my life. I must master it as I must master my life.\nMy discovery, without me, is useless. Without my discovery, I am useless. I must fire my\ndiscovery true. I must shoot straighter than my enemy who is trying to kill me. I must\nshoot him before he shoots me. I will...\nMy discovery and I know that what counts in war is not the rounds we fire, the noise of\nour burst, nor the smoke we make. We know that it is the hits that count. We will hit...\nMy discovery is human, even as I, because it is my life. Thus, I will learn it as a brother.\nI will learn its weaknesses, its strength, its parts, its accessories, its sights and its\nbarrel. I will keep my discovery clean and ready, even as I am clean and ready. We will\nbecome part of each other. We will...\nBefore God, I swear this creed. My discovery and I are the defenders of my country. We are\nthe masters of our enemy. We are the saviors of my life.\nSo be it, until victory is America's and there is no enemy, but peace!\n",strVal);
	
	/*
	printf("Num args: %d\n", argc);
	for(int i = 0; i < argc; ++i) {
		printf("Arg %d: %s\n", i, argv[i]);
	}
	*/
	HDWF * pDevice = malloc(sizeof(HDWF) * 1);
	FDwfDeviceOpen(devIdx, pDevice);
	return pDevice;
}

void resetDevice(HDWF device) {
	//Resets entire device
	FDwfDeviceReset(device);
}

void closeAllDevices() {
	FDwfDeviceCloseAll();
}

int main(int argc, char *argv[]) {
	int channel = 0, bufSize;
	
	HDWF * pDevice = getDevice();
	HDWF device = *pDevice;

	resetDevice(device);

	//Main Stuff
	//Get main device info
	getInfo(pDevice);
	//Set measurements: instruments to use and instrument parameters
	configure(pDevice, channel, &bufSize);
	//Get measurements
	run(pDevice, channel, bufSize);

	//Clean up
	closeAllDevices();
	return 0;
}
