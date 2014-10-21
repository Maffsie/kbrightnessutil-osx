/*
 * kbbutil 0.1
 * A very basic application for controlling the keyboard backlight on OSX-running MacBooks.
 * Copyright 2014, Matthew Connelly.
 */

// includes {{{
#include "kbbutil.h"
//}}}

// variable defs {{{
io_connect_t io;
uint64_t wtf=0;
//}}}

int getKBBrightness() {//{{{
	kern_return_t kr;
	uint32_t c_out=1,c_in=1;
	uint64_t cbr;
	kr = IOConnectCallScalarMethod(io,kGetLEDBrightnessID,&wtf,c_in,&cbr,&c_out);
	if(kr==kIOReturnBusy) {fprintf(stderr,"AppleLMUController IO is busy?\n");return -1;}
	if(kr!=KERN_SUCCESS) {mach_error("IOKit error: ",kr);exit(kr);}
	return (int)cbr;
}//}}}
bool setKBBrightness(int nbr) {//{{{
	kern_return_t kr;
	uint32_t c_out=1,c_in=2;
	uint64_t cbr,sbr[2]={wtf,nbr};
	kr = IOConnectCallScalarMethod(io,kSetLEDBrightnessID,sbr,c_in,&cbr,&c_out);
	if(kr==kIOReturnBusy) {fprintf(stderr,"AppleLMUController IO is busy?\n");return false;}
	if(kr!=KERN_SUCCESS) {mach_error("IOKit error: ",kr);exit(kr);}
	return true;
}//}}}

int main(int argc, char *argv []) {//{{{
	kern_return_t kr;
	io_service_t is;
	is = IOServiceGetMatchingService(kIOMasterPortDefault,IOServiceMatching("AppleLMUController"));
	if(!is) {fprintf(stderr,"Failed to into AppleLMUController\n");exit(1);}
	kr = IOServiceOpen(is,mach_task_self(),0,&io);
	IOObjectRelease(is);
	if(kr!=KERN_SUCCESS) {mach_error("IOKit error: ",kr);exit(kr);}
	if(argc==1||(argc==2&&(strcmp(argv[1],"-n")==0||strcmp(argv[1],"--numeric-only")==0))) printf(argc==2? "%i" : "Current keyboard brightness: %i\n",getKBBrightness());
	else if(argc==2&&((atoi(argv[1])==0&&strcmp(argv[1],"0")==0)||(atoi(argv[1])>0&&atoi(argv[1])<=4091))) setKBBrightness(atoi(argv[1]));
	else {fprintf(stderr,"Invalid argument. kbbutil must be run either with no arguments, with -n or --numeric-only, or with one argument which is a number between 0 and 4091.\n");exit(1);}
	return 0;
}//}}}
