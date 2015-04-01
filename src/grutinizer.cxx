

#include <cstdio>
#include <string>
#include <sys/stat.h>
#include <netdb.h>

#include "TEnv.h"
#include "TPluginManager.h"
#include "TGRUTint.h"

#include "GRUTVersion.h"

//#include "bits/predefs.h"

/*

#ifdef __APPLE__
#define HAVE_UTMPX_H
#define UTMP_NO_ADDR
#ifndef ut_user
#   define ut_user ut_name
#endif
#ifndef UTMP_FILE
#define UTMP_FILE "/etc/utmp"
#endif
#endif


# ifdef HAVE_UTMPX_H
# include <utmpx.h>
# define STRUCT_UTMP struct utmpx
# else
# if defined(__linux) && defined(__powerpc) && (__GNUC__ == 2) && (__GNUC_MINOR__ < 90)
   extern "C" {
# endif
# include <utmp.h>
# define STRUCT_UTMP struct utmp
# endif

*/

//static STRUCT_UTMP *gUtmpContents;

void SetGRUTEnv();
void SetGRUTPluginHandlers();
//static int ReadUtmp();
//static STRUCT_UTMP *SearchEntry(int, const char*);
//static void SetDisplay();






int main(int argc, char **argv) {
   //Find the grsisort environment variable so that we can read in .grsirc
   //SetDisplay();
   SetGRUTEnv();
   SetGRUTPluginHandlers();
   TGRUTint *input = 0;

   //Create an instance of the grsi interpreter so that we can run root-like interpretive mode
   input = TGRUTint::instance(argc,argv);
   //input->GetOptions(&argc,argv);
   //Run the code!
   input->Run("true");
   //Be polite when you leave.
   printf("\nbye,bye\n");

   return 0;
}


void SetGRUTEnv() {
   std::string grut_path = getenv("GEBSYS"); //Finds the GRSISYS path to be used by other parts of the grsisort code
   if(grut_path.length()>0) {
      grut_path += "/";
   }
   //Read in grsirc in the GRSISYS directory to set user defined options on grsisort startup
   grut_path +=  ".grutrc"; 
   gEnv->ReadFile(grut_path.c_str(),kEnvChange);
}

void SetGRUTPluginHandlers() {
   //gPluginMgr->AddHandler("GRootCanvas","grsi","GRootCanvas"
   gPluginMgr->AddHandler("TGuiFactory","root","GROOTGuiFactory","Gui","GROOTGuiFactory()");
   gPluginMgr->AddHandler("TBrowserImp","GRootBrowser","GRootBrowser",
                          "Gui","NewBrowser(TBrowser *,const char *,Int_t,Int_t,UInt_t,UInt_t");
   gPluginMgr->AddHandler("TBrowserImp","GRootBrowser","GRootBrowser",
                          "Gui","NewBrowser(TBrowser *,const char *,Int_t,Int_t");
}

/*

static int ReadUtmp() {
   FILE  *utmp;
   struct stat file_stats;
   size_t n_read, size;

   gUtmpContents = 0;

   utmp = fopen(UTMP_FILE, "r");
   if (!utmp)
      return 0;

   fstat(fileno(utmp), &file_stats);
   size = file_stats.st_size;
   if (size <= 0) {
      fclose(utmp);
      return 0;
   }

   gUtmpContents = (STRUCT_UTMP *) malloc(size);
   if (!gUtmpContents) {
      fclose(utmp);
      return 0;
   }

   n_read = fread(gUtmpContents, 1, size, utmp);
   if (!ferror(utmp)) {
      if (fclose(utmp) != EOF && n_read == size)
         return size / sizeof(STRUCT_UTMP);
   } else
      fclose(utmp);

   free(gUtmpContents);
   gUtmpContents = 0;
   return 0;
}

static STRUCT_UTMP *SearchEntry(int n, const char *tty)  {
   STRUCT_UTMP *ue = gUtmpContents;
   while (n--) {
      if (ue->ut_name[0] && !strncmp(tty, ue->ut_line, sizeof(ue->ut_line)))
         return ue;
      ue++;
   }
   return 0;
}


static void SetDisplay()  {
   // Set DISPLAY environment variable.

   if (!getenv("DISPLAY")) {
      char *tty = ttyname(0);  // device user is logged in on
      if (tty) {
         tty += 5;             // remove "/dev/"
         STRUCT_UTMP *utmp_entry = SearchEntry(ReadUtmp(), tty);
         if (utmp_entry) {
            char *display = new char[sizeof(utmp_entry->ut_host) + 15];
            char *host = new char[sizeof(utmp_entry->ut_host) + 1];
            strncpy(host, utmp_entry->ut_host, sizeof(utmp_entry->ut_host));
            host[sizeof(utmp_entry->ut_host)] = 0;
            if (host[0]) {
               if (strchr(host, ':')) {
                  sprintf(display, "DISPLAY=%s", host);
                  fprintf(stderr, "*** DISPLAY not set, setting it to %s\n",
                          host);
               } else {
                  sprintf(display, "DISPLAY=%s:0.0", host);
                  fprintf(stderr, "*** DISPLAY not set, setting it to %s:0.0\n",
                          host);
               }
               putenv(display);
#ifndef UTMP_NO_ADDR
            } else if (utmp_entry->ut_addr) {
               struct hostent *he;
               if ((he = gethostbyaddr((const char*)&utmp_entry->ut_addr,
                                       sizeof(utmp_entry->ut_addr), AF_INET))) {
                  sprintf(display, "DISPLAY=%s:0.0", he->h_name);
                  fprintf(stderr, "*** DISPLAY not set, setting it to %s:0.0\n",
                          he->h_name);
                  putenv(display);
               }
#endif
            }
            delete [] host;
            // display cannot be deleted otherwise the env var is deleted too
         }
         free(gUtmpContents);
      }
   }
}

*/








