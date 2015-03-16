#ifndef GLOBALS_H
#define GLOBALS_H

#ifndef __CINT__

//#define GRSI_RELEASE "2.3.22 stable"

#define RESET_COLOR "\e[m"
#define BLUE "\e[1;34m"
#define YELLOW "\e[1;33m"
#define GREEN "\e[1;32m"
#define RED "\e[1;31m"
#define BLACK "\e[1;30m"
#define MAGENTA "\e[1;35m"
#define CYAN "\e[1;36m"
#define WHITE "\e[1;37m"

#define DBLUE "\e[0;34m"
#define DYELLOW "\e[0;33m"
#define DGREEN "\e[0;32m"
#define DRED "\e[0;31m"
#define DBLACK "\e[0;30m"
#define DMAGENTA "\e[0;35m"
#define DCYAN "\e[0;36m"
#define DWHITE "\e[0;37m"

#define BG_WHITE   "\e[47m"
#define BG_RED     "\e[41m"
#define BG_GREEN   "\e[42m"
#define BG_YELLOW  "\e[43m"
#define BG_BLUE    "\e[44m"
#define BG_MAGENTA "\e[45m"
#define BG_CYAN    "\e[46m"

#define HIDE_CURSOR "\e[?25l"
#define SHOW_CURSOR "\e[?25h"

#define ALERTTEXT "\e[47m\e[0;31m"

#define NUM_SIS_CHAN 8

#define MAXSAMPLESIZE 8192 

#define FRAGMENTBUFFERSIZE 1000

#define BUILDINGTIMECONDITION 2
#define BUILDINGTRIGGERCONDITION 999

#include <string>

#if __APPLE__
#ifdef __CINT__
#undef __GNUC__
typedef char __signed;
typedef char int8_t;
#endif
#endif

#include <string>
#include <cstdio>
#include <cstdlib>
#include <stdint.h>
struct MNEMONIC {
  int16_t arrayposition;
  int16_t segment;
  std::string system;
  std::string subsystem;
  std::string arraysubposition;
  std::string collectedcharge;
  std::string outputsensor;
};


static void ParseMNEMONIC(std::string *name,MNEMONIC *mnemonic) {
	if(!name || name->length()<9)
		return;
   std::string buf;
   mnemonic->system.assign(*name,0,2);
   mnemonic->subsystem.assign(*name,2,1);
   buf.clear(); buf.assign(*name,3,2);
   mnemonic->arrayposition = (uint16_t)atoi(buf.c_str());
   mnemonic->arraysubposition.assign(*name,5,1);
   mnemonic->collectedcharge.assign(*name,6,1);
   buf.clear(); buf.assign(*name,7,2);
   mnemonic->segment = (uint16_t)atoi(buf.c_str());
   mnemonic->outputsensor.assign(*name,9,1);
   return;
}

static void ParseMNEMONIC(const char *name,MNEMONIC *mnemonic) {
   std::string sname = name;
   ParseMNEMONIC(&sname,mnemonic);
   return;
}

static void PrintMNEMONIC(MNEMONIC *mnemonic) {
	if(!mnemonic)
		return;
	printf("======MNEMONIC ======\n");
	printf("arrayposition    = %i\n", mnemonic->arrayposition);
	printf("segment          = %i\n", mnemonic->segment);
	printf("system           = %s\n", mnemonic->system.c_str());
	printf("subsystem        = %s\n", mnemonic->subsystem.c_str());
	printf("arraysubposition = %s\n", mnemonic->arraysubposition.c_str());
	printf("collectedcharge  = %s\n", mnemonic->collectedcharge.c_str());
	printf("outputsensor     = %s\n", mnemonic->outputsensor.c_str());
	printf("===============================\n");
	return;
}

static void ClearMNEMONIC(MNEMONIC *mnemonic) {
   if(!mnemonic)
      return;

   mnemonic->arrayposition = -1;
   mnemonic->segment       = -1;
   mnemonic->system.clear();
   mnemonic->subsystem.clear();
   mnemonic->arraysubposition.clear();
   mnemonic->collectedcharge.clear();
   mnemonic->outputsensor.clear();
}


const std::string &ProgramName(void);

#endif
#endif
