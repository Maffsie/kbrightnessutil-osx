/*
 * kbbutil.h - header file for kbbutil.c
 */

// includes {{{
#include <mach/mach.h>
#include <stdio.h>
#include <stdlib.h>
#include <IOKit/IOKitLib.h>
//}}}

// prototypes {{{
int getKBBrightness(void);
bool setKBBrightness(int);
enum {
	kGetSensorReadingID	= 0,
	kGetLEDBrightnessID	= 1,
	kSetLEDBrightnessID = 2,
	kSetLEDFadeID		= 3,
};
//}}}
