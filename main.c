/*
	Author: Michael Christen
	Purpose: Interface with Digilent SDK
	Date: 7/20/2014
*/
#include "main.h"
#include<digilent/waveforms/dwf.h>


int main(int argc, char *argv[]) {
	int numDevices = 0;
	FDwfEnum(enumfilterAll, &numDevices);
	printf("Num devices: %d\n", numDevices);
	printf("Num args: %d\n", argc);
	for(int i = 0; i < argc; ++i) {
		printf("Arg %d: %s\n", i, argv[i]);
	}
	return 0;
}
