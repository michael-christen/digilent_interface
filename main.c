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


int main(int argc, char *argv[]) {
	int numDevices = 0;
	FDwfEnum(enumfilterAll, &numDevices);
	/*
	printf("Num devices: %d\n", numDevices);
	printf("Num args: %d\n", argc);
	for(int i = 0; i < argc; ++i) {
		printf("Arg %d: %s\n", i, argv[i]);
	}
	*/
	HDWF device;
	FDwfDeviceOpen(-1, &device);
	//Start stuff
	FDwfAnalogInReset(device);
	FDwfAnalogInAcquisitionModeSet(device, acqmodeSingle);
	double frequency = 100000;
	const int channel = 1;
	int bufSize = 5000;

	FDwfAnalogInFrequencySet(device, frequency);
	FDwfAnalogInChannelEnableSet(device, channel, TRUE);
	FDwfAnalogInChannelRangeSet(device, channel, 2);

	FDwfAnalogInBufferSizeSet(device, bufSize);
	//Get actual buffer size
	FDwfAnalogInBufferSizeGet(device, &bufSize);
	FDwfAnalogInFrequencyGet(device, &frequency);
	FDwfAnalogInRecordLengthSet(device, bufSize / frequency);

	sleep(1);

	//Start
	FDwfAnalogInConfigure(device, TRUE, TRUE);
	DwfState devState = DwfStateRunning;
	//Finish
	while(devState != DwfStateDone) {
		FDwfAnalogInStatus(device, TRUE, &devState);
	}
	//get data
	double * data = malloc(sizeof(double) * bufSize);
	FDwfAnalogInStatusData(device,channel, data, bufSize); 
	for(int i = 0; i < bufSize; ++i) {
		printf("%d\t%f\n", i, data[i]);
	}

	//Stop instrument
	FDwfAnalogInConfigure(device, FALSE, FALSE);

	//free data & close
	free(data);
	FDwfDeviceCloseAll();
	return 0;
}
