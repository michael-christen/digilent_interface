#include<digilent/waveforms/dwf.h>
#include "main.h"

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

	FDwfAnalogInBitsInfo(device, &genIntVal);
	printf("Num Bits: %d\n", genIntVal);

	double minVolts, maxVolts, rSteps;
	FDwfAnalogInChannelRangeInfo(device, &minVolts, &maxVolts, &rSteps);
	printf("[RANGE] min: %f, max: %f, steps: %f\n",minVolts,maxVolts, rSteps);
	int numSteps;
	double rangeSteps[32];
	FDwfAnalogInChannelRangeSteps(device, rangeSteps, &numSteps);
	printf("Num Steps: %d\n",numSteps);
	for(int i = 0; i < 32; ++i) {
		printf("Step[%d]: %f\n",i,rangeSteps[i]);
	}

	FDwfAnalogInChannelOffsetInfo(device, &minVolts, &maxVolts, &rSteps);
	printf("[OFFSET] min: %f, max: %f, steps: %f\n",
			minVolts, maxVolts, rSteps);

	printf("--------------\nDigital\n\n");
	FDwfDigitalInBitsInfo(device, &genIntVal);
	printf("Num Digital Bits: %d\n", genIntVal);
	double genDouble;
	FDwfDigitalInInternalClockInfo(device, &genDouble);
	printf("Internal Clock Frequency: %f\n",genDouble);
	FDwfDigitalInClockSourceInfo(device, &genIntVal);
	printf("ClockSource %x\n",genIntVal);
	if(IsBitSet(genIntVal, DwfDigitalInClockSourceInternal)){
		printf("Digital Internal clock\n");
	} if(IsBitSet(genIntVal, DwfDigitalInClockSourceExternal)) {
		printf("Digital External clock\n");
	}
	unsigned int genUInt;
	FDwfDigitalInDividerInfo(device, &genUInt);
	printf("Max clock divider: %u\n", genUInt);

	FDwfDigitalInBufferSizeInfo(device, &genIntVal);
	printf("Max Digital BufferSize: %d\n", genIntVal);

	FDwfDigitalInSampleModeInfo(device, &genIntVal);
	if(IsBitSet(genIntVal, DwfDigitalInSampleModeSimple)){
		printf("Simple Sample Mode supported\n");
	} if(IsBitSet(genIntVal, DwfDigitalInSampleModeNoise)) {
		printf("Noise Sample Mode supported\n");
	}

	FDwfDigitalInAcquisitionModeInfo(device, &genIntVal);
	if(IsBitSet(genIntVal, acqmodeSingle)) {
		printf("Acq single\n");
	} if(IsBitSet(genIntVal, acqmodeScanShift)) {
		printf("Acq scan shift\n");
	} if(IsBitSet(genIntVal, acqmodeScanScreen)) {
		printf("Acq scan screen\n");
	}

}


void configure(HDWF * pDevice, int channel, int * bufSize) {

	int genIntVal;
	HDWF device = *pDevice;

	//Autoconfigure (configures after every set if true)
	FDwfDeviceAutoConfigureSet(device, FALSE);
	BOOL genBool;
	FDwfDeviceAutoConfigureGet(device, &genBool);
	printf("Autonfigure: %d\n",genBool);

	//Reset all AnalogIn Instruments
	FDwfAnalogInReset(device);
	//Set Parameters
	double frequency = 100000;
	*bufSize = 5000;
	FDwfAnalogInAcquisitionModeSet(device, acqmodeScanScreen);
	FDwfAnalogInFrequencySet(device, frequency);
	FDwfAnalogInChannelEnableSet(device, channel, TRUE);

	double rSteps;
	FDwfAnalogInChannelRangeSet(device, channel, 2);
	FDwfAnalogInChannelRangeGet(device, channel, &rSteps);
	printf("Range set to %f\n",rSteps);

	//FDwfAnalogInChannelOffsetSet(device, channel, -5);
	FDwfAnalogInChannelOffsetGet(device, channel, &rSteps);
	printf("Offset set to %f\n",rSteps);

	FDwfAnalogInBufferSizeSet(device, *bufSize);
	//Get actual values
	FDwfAnalogInBufferSizeGet(device, bufSize);
	FDwfAnalogInFrequencyGet(device, &frequency);
	//FDwfAnalogInRecordLengthSet(device, *bufSize / frequency);

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
	FILE * fp = fopen("output.txt", "w");
	int dataId = 0;
	char line[4096];
	while(devState != DwfStateDone) {
		printf("Get Status\n");
		fgets(line, sizeof(line), stdin); 
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
		for(int i = 0; i < numSamples; ++i) {
			fprintf(fp, "%d\t%f\n", dataId++, data[i]);
		}

		dataId = 0;
		fprintf(fp, "\n");
		printf("Sample Call: %d\n", FDwfAnalogInStatusSample(device, channel, &dataSample));
		printf("Sample: %f\n", dataSample);
		int dataAvail, dataLost, dataCorrupt;
		FDwfAnalogInStatusRecord(device, &dataAvail, &dataLost, &dataCorrupt);
		//This is difference since last read
		printf("Avail:%d, Lost:%d, Corrupt:%d\n",
				dataAvail, dataLost, dataCorrupt);

		//Used in Scan Screen
		FDwfAnalogInStatusIndexWrite(device, &genIntVal);
		printf("Status Index Write: %d\n", genIntVal);

		double rSteps;
		FDwfAnalogInChannelRangeGet(device, channel, &rSteps);
		printf("Range set to %f\n",rSteps);

		FDwfAnalogInChannelOffsetGet(device, channel, &rSteps);
		printf("Offset set to %f\n",rSteps);
		
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



