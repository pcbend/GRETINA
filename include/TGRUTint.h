#ifndef TGRUTINT_H
#define TGRUTINT_H

#include "Globals.h"

#include <cstdio>
#include <string>

#include "TSystem.h"
#include "TSysEvtHandler.h"
#include "TRint.h"
#include "TList.h"
#include "TEnv.h"

class TGRUTint : public TRint {

   private:
      TGRUTint(int argc, char **argv,void *options = 0, 
            int numOptions = 0, bool noLogo = false, const char *appClassName = "grsisort") ;

      static TEnv *fGRSIEnv;

   public:
      static TGRUTint *fTGRUTint;
      static TGRUTint *instance(int argc = 0, char **argv = 0, void *options = 0, 
                             int numOptions = -1, bool noLogo = false, const char *appClassName = "grsisort");
      
      virtual ~TGRUTint();

      void GetOptions(int *argc,char **argv);
      void PrintHelp(bool);
      void PrintLogo(bool);
      bool HandleTermInput();
      int  TabCompletionHook(char*,int*,std::ostream&);

      //bool Sort() {return TEventLoop::Get()->SortMidas();}

      static TEnv *GetEnv() { return fGRSIEnv; }

   private:
      bool FileAutoDetect(std::string filename, long filesize);
      void InitFlags();
      void ApplyOptions();
      //void SetEnv();
      void DrawLogo();
      void LoadGROOTGraphics();

   private: 
      bool fPrintLogo;
      bool fPrintHelp;
      
      bool fAutoSort;
      bool fFragmentSort;
      bool fMakeAnalysisTree;


   ClassDef(TGRUTint,0);
};


class TGRSIInterruptHandler : public TSignalHandler {
   public:
      TGRSIInterruptHandler():TSignalHandler(kSigInterrupt,false) { }
      bool Notify();
};



#endif
