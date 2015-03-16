#ifndef __MAIN__H__
#define __MAIN__H__
#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
//The main header file, handling the running of the interface
void getInfo(HDWF * pDevice);
void configure(HDWF * pDevice, int channel, int * bufSize);
void run(HDWF * pDevice, int channel, int bufSize);

#define TRUE  1
#define FALSE 0

#endif
