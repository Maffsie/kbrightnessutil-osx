/*
 * kbbutil 0.2
 * A very basic application for controlling the keyboard backlight on OSX-running MacBooks.
 * Copyright 2014-15, Matthew Connelly.
 */

// includes {{{
#include "kbbutil.h"
//}}}

// variable defs {{{
io_connect_t io;
uint64_t wtf=0; // I don't know what this is used for
//}}}

int getKBBrightness(void) {//{{{
	kern_return_t kr;
	uint32_t c_out=1, c_in=1;
	uint64_t cbr;
	kr = IOConnectCallScalarMethod(io,kGetLEDBrightnessID,&wtf,c_in,&cbr,&c_out);
	if(kr==kIOReturnBusy) { fprintf(stderr,"AppleLMUController IO is busy?\n"); return -1; }
	if(kr!=KERN_SUCCESS)  { mach_error("IOKit error: ",kr); exit(kr); }
	return (int)cbr;
}//}}}
bool setKBBrightness(int nbr) {//{{{
	kern_return_t kr;
	uint32_t c_out=1,c_in=2;
	uint64_t cbr,sbr[2]={wtf,nbr};
	kr = IOConnectCallScalarMethod(io,kSetLEDBrightnessID,sbr,c_in,&cbr,&c_out);
	if(kr==kIOReturnBusy) { fprintf(stderr,"AppleLMUController IO is busy?\n"); return false; }
	if(kr!=KERN_SUCCESS)  { mach_error("IOKit error: ",kr); exit(kr); }
	return true;
}//}}}

int main(int argc, char *argv []) {//{{{
	kern_return_t kr;
	io_service_t is;
	is = IOServiceGetMatchingService(kIOMasterPortDefault,IOServiceMatching("AppleLMUController"));
	if(!is) { fprintf(stderr,"Failed to get AppleLMUController service\n"); exit(1); }
	kr = IOServiceOpen(is,mach_task_self(),0,&io);
	IOObjectRelease(is);
	if(kr!=KERN_SUCCESS) { mach_error("IOKit error: ",kr); exit(kr); }
	//big argparsing block i intend to clean up later, but for now i've just formatted it more readably
	if
		(argc==1 ||
		(argc==2 &&
			(strcmp(argv[1],"-n")==0 ||
			strcmp(argv[1],"--numeric-only")==0)
		)
	)
		printf(argc==2? "%i\n" : "Current keyboard brightness: %i\n",getKBBrightness());
	else if
		(argc==2 && (
			(atoi(argv[1])==0 && strcmp(argv[1],"0")==0) ||
			(atoi(argv[1])> 0 && atoi(argv[1])<=4091)
		)
	) {
		if(!setKBBrightness(atoi(argv[1]))) {
			fprintf(stderr,"Error setting brightness to %i.\n",atoi(argv[1]));
			exit(1);
		}
	} else {
		fprintf(stderr,"Usage:\n\
- kbbutil : Returns current brightness\n\
- kbbutil [-n|--numeric-only] : Returns only integer brightness\n\
- kbbutil [0-4091] : Set brightness\n");
		exit(1);
	}
	return 0;
}//}}}
