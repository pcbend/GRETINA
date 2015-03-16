

#include "TGRUTint.h"
//#include "TGRSILoop.h"

#include "TGRUTOptions.h"

//#include "TGRSIRootIO.h"
//#include "TDataParser.h"
//#include "TAnalysisTreeBuilder.h"
#include "Getline.h"
#include "GROOTGuiFactory.h"

#include "Globals.h"
#include "GRUTVersion.h"

#include "TROOT.h"
#include "TFile.h"
//#include "TGHtmlBrowser.h"
//#include <pstream.h>

#include <thread>

ClassImp(TGRUTint)

extern void PopupLogo(bool);
extern void WaitLogo();

TGRUTint *TGRUTint::fTGRUTint = NULL;

TEnv *TGRUTint::fGRSIEnv = NULL;
//std::vector<std::string> *TGRUTint::fInputRootFile  = new std::vector<std::string>;
//std::vector<std::string> *TGRUTint::fInputMidasFile = new std::vector<std::string>;
//std::vector<std::string> *TGRUTint::fInputCalFile   = new std::vector<std::string>;
//std::vector<std::string> *TGRUTint::fInputOdbFile   = new std::vector<std::string>;

void ReadTheNews(void);

TGRUTint *TGRUTint::instance(int argc,char** argv, void *options, int numOptions, bool noLogo, const char *appClassName) {
   if(!fTGRUTint)
      fTGRUTint = new TGRUTint(argc,argv,options,numOptions,true,appClassName);
   return fTGRUTint;
}

TGRUTint::TGRUTint(int argc, char **argv,void *options, Int_t numOptions, Bool_t noLogo,const char *appClassName) 
      :TRint(appClassName, &argc, argv, options, numOptions,noLogo) {

      fGRSIEnv = gEnv;
      //TRint::TRint(appClassName, &argc, argv, options, numOptions,noLogo)

      //TSignalHandler sig_handi;
      GetSignalHandler()->Remove();
      TGRSIInterruptHandler *ih = new TGRSIInterruptHandler();
      ih->Add();

      InitFlags();
      GetOptions(&argc,argv);
      PrintLogo(fPrintLogo);
    //  SetPrompt( DYELLOW "GRSI [%d] " RESET_COLOR);
      SetPrompt("GRSI [%d] ");
      PrintHelp(fPrintHelp);
      ApplyOptions();
}


void TGRUTint::InitFlags() {
   fAutoSort = false;
   fFragmentSort = false;
   fMakeAnalysisTree = false;

//   if(fGRSIEnv) fGRSIEnv->Delete();
}

void TGRUTint::ApplyOptions() {
   
   if(!false) { // this will be change to something like, if(!ClassicRoot) 
      LoadGROOTGraphics();
   }

   if(TGRUTOptions::ReadingMaterial()) {
      //std::thread fnews = std::thread(ReadTheNews);
      //fnews.detach();
   }


   if(fAutoSort){
     //TGRSILoop::Get()->SortMidas();
   }
   bool foundCal = false;
   if(fFragmentSort && TGRUTOptions::GetInputRoot().size()!=0)
      //TGRSIRootIO::Get()->MakeUserHistsFromFragmentTree();
   if(TGRUTOptions::MakeAnalysisTree() && TGRUTOptions::GetInputRoot().size()!=0) { 
      //TAnalysisTreeBuilder::Get()->StartMakeAnalysisTree();
   }
   for(int x=0;x<TGRUTOptions::GetInputRoot().size();x++) {
      //printf("TFile *_file%i = new TFile(\"%s\",\"read\")\n",x,TGRUTOptions::GetInputRoot().at(x).c_str());
      long error = ProcessLine(Form("TFile *_file%i = new TFile(\"%s\",\"read\");",x,TGRUTOptions::GetInputRoot().at(x).c_str()));
      if(error <=0) continue;
      TFile *file = (TFile*)gROOT->FindObject(TGRUTOptions::GetInputRoot().at(x).c_str());
      printf("\tfile %s opened as _file%i\n",file->GetName(),x);
      //TGRSIRootIO::Get()->LoadRootFile(file);
   }
   if(TGRUTOptions::GetInputRoot().size() > 0) {
      if(TGRUTOptions::GetInputRoot().at(0).find("fragment") != std::string::npos){
         Int_t chans_read = ProcessLine("TChannel::ReadCalFromTree(FragmentTree)");
         printf("Read calibration info for %d channels from \"%s\" FragmentTree\n",chans_read,TGRUTOptions::GetInputRoot().at(0).c_str()); 
      }   
      if(TGRUTOptions::GetInputRoot().at(0).find("analysis") != std::string::npos){ 
         Int_t chans_read = ProcessLine("TChannel::ReadCalFromTree(AnalysisTree)");    
         printf("Read calibration info for %d channels from \"%s\" AnalysisTree\n",chans_read,TGRUTOptions::GetInputRoot().at(0).c_str());
      }
   }
  
  if(TGRUTOptions::WorkHarder()) {
      for(int x=0;x<TGRUTOptions::GetMacroFile().size();x++) {
         gROOT->Macro(TGRUTOptions::GetMacroFile().at(x).c_str());  
       // gROOT->ProcessLineSync(Form(".x %s",TGRUTOptions::GetMacroFile().at(x).c_str()));
      }
   }


  if(TGRUTOptions::CloseAfterSort())
     gApplication->Terminate();
}


TGRUTint::~TGRUTint()   {
// SafeDelete();
}

bool TGRUTint::HandleTermInput() {
   return TRint::HandleTermInput();
}

int TGRUTint::TabCompletionHook(char* buf, int* pLoc, std::ostream& out) {
   return TRint::TabCompletionHook(buf,pLoc,out);
}

void ReadTheNews(void) {
   //gROOT->ProcessLine(".! wget -q -l1 - http://en.wikipedia.org/wiki/Special:Random -Otemp.html");
   //new TGHtmlBrowser("temp.html");
   //std::ipstream wrandom("xdg-open http://en.wikipedia.org/wiki/Special:Random");
   #ifdef __APPLE__
   gROOT->ProcessLine(".! open http://en.wikipedia.org/wiki/Special:Random > /dev/null 2>&1");
   #else
   gROOT->ProcessLine(".! xdg-open http://en.wikipedia.org/wiki/Special:Random > /dev/null 2>&1");
   #endif
   return;
}

void TGRUTint::PrintLogo(bool print) {

   if(print)   {
      const std::string &ref = "Sorting Program for Online and Offline Nuclear Data";
      const unsigned int reflength = 53;

     const unsigned int width = reflength + (reflength % 2);
     printf("\t*%s*\n", std::string(width,'*').c_str());   
     printf("\t*%*s%*s*\n",width/2+4,"GRUT Sort", width/2-4, "");
     printf("\t*%*s%*s*\n",width/2+12,"a remake of a remake ", width/2-12, "");
     printf("\t*%*s%*s*\n",width/2+reflength/2, ref.c_str(), width/2-reflength/2, "");
     printf("\t*%*s%*s*\n",width/2+14,"A lean, mean sorting machine", width/2-14, "");
     printf("\t*%*s%*s*\n",width/2+9,"version " GRUT_RELEASE, width/2-9, "");
     printf("\t*%s*\n", std::string(width,'*').c_str());   

     std::thread drawlogo(&TGRUTint::DrawLogo,this);
     drawlogo.detach();
   }
}

void TGRUTint::DrawLogo() {
   PopupLogo(false);
   WaitLogo();
}

void TGRUTint::GetOptions(int *argc, char **argv) {

   static char null[1] = { "" };

   fPrintLogo = true;
   fPrintHelp = false;

   if(!argc)
      return;

   std::string pwd ="";

   for (int i = 1; i < *argc; i++) {        //HELP!
      std::string sargv = argv[i];
      if(sargv.length()<2) {
         // one char is not enough to be an option.
         if(sargv[0] == '-') 
           printf(DBLUE "   found option flag '-' not immediately followed by an option." RESET_COLOR "\n");
         else   
           printf(DBLUE "   stand alone option %s not understood, skipping." RESET_COLOR "\n", sargv.c_str());
      }
      if (!strcmp(argv[i],"-?") || !strncmp(argv[i], "--help", 6)) {
         fPrintHelp = true;
      } else if(!strcmp(argv[i],"-h") || !strcmp(argv[i],"-H")) { 
        if(sargv.length()==2) {
          i++; 
          if(i >= *argc) {
            printf(DBLUE "   -h flag given with no host name!" RESET_COLOR "\n");
            break;
          }
          sargv.assign(argv[i]);
          if(sargv[0] == '-' || sargv[0] == '+') {
            i--;
            printf(DRED "     invalid host name: %s; ignoring." RESET_COLOR  "\n",sargv.c_str());
            break;
          } 
        } else {
          sargv = sargv.substr(2);
        }  
        TGRUTOptions::SetHostName(sargv);      
        printf(DYELLOW "host: %s" RESET_COLOR "\n",sargv.c_str());
        break;
      } else if(!strcmp(argv[i],"-e") || !strcmp(argv[i],"-E")) {
        if(sargv.length()==2) {
          i++; 
          if(i >= *argc) {
            printf(DBLUE "   -e flag given with no expt name!" RESET_COLOR "\n");
            break;
          }
          sargv.assign(argv[i]);
          if(sargv[0] == '-' || sargv[0] == '+') {
            i--;
            printf(DRED "     invalid host expt: %s; ignoring." RESET_COLOR  "\n",sargv.c_str());
            break;
          } 
        } else {
          sargv = sargv.substr(2);
        }
        TGRUTOptions::SetExptName(sargv);      
        printf(DYELLOW "experiment: %s" RESET_COLOR "\n",sargv.c_str());
        break;
      } else if (sargv[0] == '-' && sargv[1] != '-') { //single char options.
        sargv = sargv.substr(1);  //drop the minus;
        int defaultcounter = 0;
        for(int c=0;c<sargv.length();c++) {
          char key = sargv[c];
          switch(toupper(key)) {
            case 'A':
      		  printf(DBLUE "Atempting to make analysis trees." RESET_COLOR "\n");
         	  TGRUTOptions::SetMakeAnalysisTree();
		        break;
            case 'Q':
              printf(DBLUE "Closing after Sort." RESET_COLOR "\n");
              TGRUTOptions::SetCloseAfterSort();
              break;
            case 'L':
              fPrintLogo = false;       
              //argv[i] = null;
              break;
            case 'S':
              printf(DBLUE "SORT!!" RESET_COLOR "\n");
              fFragmentSort = true;
              break;
            case 'H':
              printf(DBLUE "Option \"h\" found in list, but must be followed by host name; skipping!\n" RESET_COLOR);
              break;
            case 'E':
              printf(DBLUE "Option \"e\" found in list, but must be followed by experiment name; skipping!\n" RESET_COLOR);
              break;
            default:
              printf(DBLUE "   option %c found but not understood, skipping." RESET_COLOR "\n", sargv[c]);
              defaultcounter++;
              if(defaultcounter>1) {
                printf("Perhaps you are trying to use a word length argument?\n");
                printf("if so, use -- in front of the word instead\n.");
                fPrintHelp = true;
                c = sargv.length() + 1;
                i = *argc + 1;   
              }   
              break;
          }
        }
     } else if (sargv[0] == '-' && sargv[1] == '-') { //word length options.
       std::string temp = sargv.substr(2);
       if(temp.compare("no_waveforms")==0) {
          printf(DBLUE  "    no waveform option set, no waveforms will be in the output tree." RESET_COLOR "\n"); 
          //TDataParser::SetNoWaveForms(true);
       } else if(temp.compare("record_stats")==0) { 
          printf(DBLUE "     recording run stats to log file." RESET_COLOR "\n");
          //TDataParser::SetRecordStats(true);
       } else if((temp.compare("suppress_error")==0) ||  (temp.compare("suppress_errors")==0)){
          printf(DBLUE "     suppressing loop error statements." RESET_COLOR "\n");
          //TGRSILoop::Get()->SetSuppressError(true);
       } else if(temp.compare("log_errors")==0) {
          printf(DBLUE "     sending parsing errors to file." RESET_COLOR "\n");
          TGRUTOptions::SetLogErrors(true);
       } else if(temp.compare("work_harder")==0) {
          printf(DBLUE "     running a macro with .x after making fragment/analysistree." RESET_COLOR "\n");
          TGRUTOptions::SetWorkHarder(true);
       } else if(temp.compare("reading_material")==0) {
          printf(DBLUE"      now providing reading material while you wait." RESET_COLOR "\n");
          TGRUTOptions::SetReadingMaterial(true);
       } else if(temp.compare("no_speed")==0) {
          printf(DBLUE "    not opening the PROOF speedometer." RESET_COLOR "\n");
          TGRUTOptions::SetProgressDialog(false);
       } else if(temp.compare("help")==0) {
          fPrintHelp = true;
       } else if(temp.compare("ignore_odb")==0) { 
          // useful when dealing with midas file that have corrupt odbs in them .
          TGRUTOptions::SetIgnoreFileOdb(true);          
       } else {
          printf(DBLUE  "    option: " DYELLOW "%s " DBLUE "passed but not understood." RESET_COLOR "\n",temp.c_str());
       }
     } else if (sargv[0] != '-' && sargv[0] != '+') { //files and directories!
       long size;
       long id, flags, modtime;
       char *dir = gSystem->ExpandPathName(argv[i]);
       if (!gSystem->GetPathInfo(dir, &id, &size, &flags, &modtime)) {   
          if ((flags & 2)) {                                                      
             //I am not sur what to do with directorys right now.                           
             //if (pwd == "") {
                pwd = argv[i]; 
                argv[i]= null;
             //} 
               printf("\tOption %s is a directory, ignoing for now.\n",pwd.c_str()); 
          } else if (size > 0) {
             // if file add to list of files to be processed
             FileAutoDetect(argv[i],size);
             argv[i] = null;
          } else {
             printf("file %s has size 0, skipping\n", dir);
          }
       } else {
          //file does not exsist... assuming output file.
          FileAutoDetect(argv[i],-1);
          argv[i] = null;
       }   
    }
  } 
}



void TGRUTint::LoadGROOTGraphics() {
   if (gROOT->IsBatch()) return;
   // force Canvas to load, this ensures global GUI Factory ptr exists.
   gROOT->LoadClass("TCanvas", "Gpad");
   gGuiFactory =  new GROOTGuiFactory();  
}


void TGRUTint::PrintHelp(bool print) {
   if(print) {
      printf( DRED BG_WHITE "     Sending Help!!     " RESET_COLOR  "\n");
      //new TGHtmlBrowser(gSystem->ExpandPathName("${GRSISYS}/README.html"));
   }
   return;
}

bool TGRUTint::FileAutoDetect(std::string filename, long filesize) {
   //first search for extensions.
   std::string ext = filename.substr(filename.find_last_of('.')+1);
   //printf("\text = %s\n",ext.c_str());
   if(ext.compare("root")==0) {
      //printf("\tFound root file: %s\n",filename.c_str());
      //fInputRootFile->push_back(filename);
      TGRUTOptions::AddInputRootFile(filename);
      return true;
   } else if(ext.compare("mid")==0 || ext.compare("bz2")==0) {
      //printf("\tFound midas file: %s\n",filename.c_str());
      //fInputMidasFile->push_back(filename);
      TGRUTOptions::AddInputMidasFile(filename);
      fAutoSort = true;
      return true;
   } else if(ext.compare("cal")==0) { 
      //printf("\tFound custom calibration file: %s\n",filename.c_str());
      //fInputCalFile->push_back(filename);
      TGRUTOptions::AddInputCalFile(filename);
      return true;
   } else if(ext.compare("xml")==0) { 
      //fInputOdbFile->push_back(filename);
      TGRUTOptions::AddInputOdbFile(filename);
      //printf("\tFound xml odb file: %s\n",filename.c_str());
      return true;
   } else if(ext.compare("odb")==0) { 
      //printf("\tFound c-like odb file: %s\n",filename.c_str());
      printf("c-like odb structures can't be read yet.\n");
      return false;
   } else if((ext.compare("c")==0) || (ext.compare("C")==0) || (ext.compare("c+")==0) || (ext.compare("C+")==0)) {
      TGRUTOptions::AddMacroFile(filename);
      return true;
   } else {
      printf("\tDiscarding unknown file: %s\n",filename.c_str());
      return false;
   }
   return false;
}


bool TGRSIInterruptHandler::Notify() {
   printf("\n" DRED BG_WHITE  "   Control-c was pressed.   " RESET_COLOR "\n");
   abort();
   gApplication->Terminate();
   return true;
}










