/*
	Author: Michael Christen
	Purpose: Interface with Digilent SDK
	Date: 7/20/2014
*/
#include <stdlib.h>
#include<digilent/waveforms/dwf.h>

#include "main.h"

#define TRUE  1
#define FALSE 0

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

void getInfo(HDWF * pDevice) {
	HDWF device = *pDevice;
	//Get Trigger Info for device
	int genIntVal;
	FDwfDeviceTriggerInfo(device, &genIntVal);
	if(IsBitSet(genIntVal,trigsrcNone))  {
	} if(IsBitSet(genIntVal, trigsrcPC)) {
	} if(IsBitSet(genIntVal, trigsrcDetectorAnalogIn)) {
	} if(IsBitSet(genIntVal, trigsrcDetectorDigitalIn)) {
	} if(IsBitSet(genIntVal, trigsrcAnalogIn)) {
	} if(IsBitSet(genIntVal, trigsrcDigitalIn)) {
	} if(IsBitSet(genIntVal, trigsrcDigitalOut)) {
	} if(IsBitSet(genIntVal, trigsrcAnalogOut1)) {
	} if(IsBitSet(genIntVal, trigsrcAnalogOut2)) {
	} if(IsBitSet(genIntVal, trigsrcAnalogOut3)) {
	} if(IsBitSet(genIntVal, trigsrcAnalogOut4)) {
	} if(IsBitSet(genIntVal, trigsrcExternal1))  {
	} if(IsBitSet(genIntVal, trigsrcExternal2)) {
	} if(IsBitSet(genIntVal, trigsrcExternal3)) {
	} if(IsBitSet(genIntVal, trigsrcExternal4)) {
	}
}


void configure(HDWF * pDevice, int channel, int * bufSize) {
	int genIntVal;
	HDWF device = *pDevice;

	//Reset all AnalogIn Instruments
	FDwfAnalogInReset(device);
	//Set Parameters
	double frequency = 100000;
	*bufSize = 5000;
	FDwfAnalogInAcquisitionModeSet(device, acqmodeSingle);
	FDwfAnalogInFrequencySet(device, frequency);
	FDwfAnalogInChannelEnableSet(device, channel, TRUE);
	FDwfAnalogInChannelRangeSet(device, channel, 2);
	FDwfAnalogInBufferSizeSet(device, *bufSize);
	//Get actual values
	FDwfAnalogInBufferSizeGet(device, bufSize);
	FDwfAnalogInFrequencyGet(device, &frequency);
	FDwfAnalogInRecordLengthSet(device, *bufSize / frequency);

	sleep(1);
}

void run(HDWF * pDevice, int channel, int bufSize) {
	HDWF device = *pDevice;
	int genIntVal;
	BOOL genBool;
	//Start
	FDwfAnalogInConfigure(device, TRUE,//TRUE resets auto trigger
		   	TRUE); //Starts acquisition
	DwfState devState = DwfStateRunning;
	//State machine running through
	double * data = malloc(sizeof(double) * bufSize);
	double dataSample;
	while(devState != DwfStateDone) {
		FDwfAnalogInStatus(device, TRUE, &devState);
		if(devState == DwfStateReady) {
			printf("Ready\n");
		} else if(devState == DwfStateConfig) {
			printf("Config\n");
		} else if(devState == DwfStatePrefill) {
			printf("Prefill\n");
		} else if(devState == DwfStateArmed) {
			printf("Armed\n");
		} else if(devState == DwfStateWait) {
			printf("Wait\n");
		} else if(devState == DwfStateTriggered) {
			printf("Triggered\n");
		} else if(devState == DwfStateRunning) {
			printf("Running\n");
		} else if(devState == DwfStateDone) {
			printf("Done\n");
		}
		/*
		switch((int)devState) {
			case (const int)DwfStateReady:
				printf("Ready\n");
				break;
			case (const int)DwfStateConfig:
				printf("Config\n");
				break;
			case (const int)DwfStatePrefill:
				printf("Prefill\n");
				break;
			case (const int)DwfStateArmed:
				printf("Armed\n");
				break;
			case (const int)DwfStateWait:
				printf("Wait\n");
				break;
			case (const int)DwfStateTriggered:
				printf("Triggered\n");
				break;
			case (const int)DwfStateRunning:
				printf("Running\n");
				break;
			case (const int)DwfStateDone:
				printf("Running\n");
				break;
		}
		*/
		FDwfAnalogInStatusSamplesLeft(device, &genIntVal);
		printf("Samples Left: %d\n", genIntVal);
		FDwfAnalogInStatusSamplesValid(device, &genIntVal);
		printf("Samples Valid: %d\n", genIntVal);
		int numSamples = genIntVal;
		FDwfAnalogInStatusAutoTriggered(device, &genBool);
		printf("Auto Triggered: %d\n", genBool);
		//Never happen on single because no valid samples
		printf("Data Call: %d\n", FDwfAnalogInStatusData(device, channel, data, numSamples));
		printf("Sample Call: %d\n", FDwfAnalogInStatusSample(device, channel, &dataSample));
		printf("Sample: %f\n", dataSample);
		int dataAvail, dataLost, dataCorrupt;
		FDwfAnalogInStatusRecord(device, &dataAvail, &dataLost, &dataCorrupt);

		//Used in Scan Screen
		FDwfAnalogInStatusIndexWrite(device, &genIntVal);
		printf("Status Index Write: %d\n", genIntVal);
		
	}
	
	//get data
	FDwfAnalogInStatusData(device,channel, data, bufSize); 
	for(int i = 0; i < bufSize; ++i) {
		printf("%d\t%f\n", i, data[i]);
	}

	//Stop instrument
	FDwfAnalogInConfigure(device, FALSE, FALSE);

	//free data & close
	free(data);

}

int main(int argc, char *argv[]) {
	
	HDWF * pDevice = getDevice();
	HDWF device = *pDevice;

	//Resets entire device
	FDwfDeviceReset(device);

	//Autoconfigure (configures after every set if true)
	FDwfDeviceAutoConfigureSet(device, FALSE);
	BOOL genBool;
	FDwfDeviceAutoConfigureGet(device, &genBool);
	printf("Autonfigure: %d\n",genBool);

	getInfo(pDevice);
	int channel = 0, bufSize;
	configure(pDevice, channel, &bufSize);
	run(pDevice, channel, bufSize);

	FDwfDeviceCloseAll();
	return 0;
}
