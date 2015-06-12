//g++ MakeTree.cxx `grut-config --cflags --libs --root` 

#include <cstdio>
#include <string>
#include <stdexcept>


#include <TGEBMultiFile.h>
#include <TGEBFile.h>
#include <TGEBEvent.h>

#include <Globals.h>

#include <Rtypes.h>

#include <TGretina.h>
#include <TPhosWall.h>

#include <LaBrFragment.h>

#include <TFile.h>
#include <TTree.h>

int main(int argc, char **argv) {
  if(argc<2) {
    printf("Usage:   MakeGretinaTree <filename.dat/dat.gz/cvt/cvt.gz> -o <outfilename.root>\n");
    printf("\tas many input files as nessecray may be passed on the commandline.\n");
    printf("\tin the case of multiple inputs, the GEB events are fromt he files\n"
           "\ttime ordered. \n");
    return 1;
  }

  TGEBMultiFile multi;
  Long_t First1 = 0; 
  Long_t First2 = 0;

  std::string outfilename;

  for(int x=1;x<argc;x++) {
    if(!strcmp(argv[x],"-o") || !strcmp(argv[x],"--outfile")) {
      if((x+1)>=argc) {
         printf("outfile flag found, but file name not found.\n");
         return 1;
      } 
      x++;
      outfilename.assign(argv[x]);
    } else {
      multi.Add(argv[x]);
    }
  }

  TGEBEvent *event = new TGEBEvent;
  size_t bytes = multi.Read(event);
  if(bytes<1) {
    bytes = multi.Read(event);
  }
  if(bytes<1) {
     printf("problem reading inputfiles.\n");
     return 1;
  }
  Long_t LastTime = event->GetTimeStamp();
  Int_t  LastType = event->GetEventType();

  int stop =0;

  TGretina  *gretina  = new TGretina;
  TPhosWall *phoswall = new TPhosWall;

  int phoscounter = 0;
  int gretcounter = 0;
  int labrcounter = 0;


  bool run = true;
  int loopcounter=0;
  multi.ShowStatus();
  while(run) {
    loopcounter++;
    try {
      bytes = multi.Read(event);
    } catch(const std::out_of_range &oor) {
       printf("bytes = %i\n",bytes);
    //   multi.ShowStatus();
       break;
    }
   
    if(bytes<0) {
       multi.ShowStatus();
       printf("bytes = %i\n",bytes);
       run =false;
       //continue;
       break;
    }   
    if(event->GetEventType()==8)
       continue;
    if(abs(event->GetTimeStamp()-LastTime)>500) {  // 5 us build time.
      //printf(DRED);
      //printf("Build called.\t");
      //std::cout << event->GetTimeStamp() << "  -  " << LastTime << "  =  " << event->GetTimeStamp() - LastTime;
      //printf(RESET_COLOR "\n");
 

      //phoswall->FindWeightedPosition();
      //gretina->BuildAddBack();
      ////phoswall->GetPosition().Print();
      //tree->Fill();      
      //gretina->Clear();
      //phoswall->Clear();
    }

    //event->Print();

    switch(event->GetEventType()) {
      case 1: {
       //printf("\tfound gretina bank 1\n");
       std::cout << *((TGEBEvent::TGEBBankType1*)event->GetData());
        G2Fragment frag(*event);
        //frag.Print();
        //TGretinaHit hit(frag);
        //gretina->AddGretinaHit(hit);
        gretcounter++;
      }
      break;
      case 17: {
        PWFragment frag(*event);
        frag.Print();
        phoswall->AddPWHit(frag);
        if(phoswall->GetTime()<0)
          printf(RED "\tFIRE\t%i\n" RESET_COLOR,phoswall->Size());
        phoscounter++;
      }
      break;
      case 18: 
        //event->Print();
        LaBrFragment frag(event);    
        labrcounter++;
        break;
    };  

    LastTime = event->GetTimeStamp();
    LastType = event->GetEventType();


    if((loopcounter%100000)==0)
       multi.Print();

  }


  return 0;

}









