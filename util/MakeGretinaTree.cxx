//g++ MakeTree.cxx `grut-config --cflags --libs --root` 

#include <cstdio>
#include <string>
#include <stdexcept>

#include <TGEBMultiFile.h>
#include <TGEBFile.h>
#include <TGEBEvent.h>

#include <TChannel.h>

#include <Globals.h>

#include <Rtypes.h>

#include <TGretina.h>
#include <TPhosWall.h>
#include <TLaBr.h>
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
  
  char *path = getenv("GEBSYS");
  std::string calfile; calfile.assign(path); calfile.append("/util/mycal.cal");
  TChannel::ReadCalFile(calfile.c_str());

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
  if(bytes < 1) {
     printf("problem reading inputfiles.\n");
     return 1;
  }

  Long_t LastTime = event->GetTimeStamp();
  Int_t  LastType = event->GetEventType();

  int stop =0;

  TGretina  *gretina  = new TGretina;
  TPhosWall *phoswall = new TPhosWall;
  TLaBr     *labr     = new TLaBr;

  int phoscounter = 0;
  int gretcounter = 0;
  int labrcounter = 0;

  if(outfilename.length()==0) {
     printf("\tno outfile file specified, using default: myoutput.root\n");
     outfilename.assign("myoutput.root");
  }


  if(outfilename.find(".root")==std::string::npos)
     outfilename.append(".root");

  TFile *outfile = new TFile(outfilename.c_str(),"recreate");
  TTree *tree = new TTree("Data","Data");
  TTree::SetMaxTreeSize((Long_t)1*(Long_t)1024*(Long_t)1024*(Long_t)1024);
  tree->Branch("TGretina","TGretina",&gretina);
  tree->Branch("TPhosWall","TPhosWall",&phoswall);
  tree->Branch("TLaBr","TLaBr",&labr);

  bool run = true;
  int loopcounter=0;

  while(run) {
    loopcounter++;
    //if(loopcounter > 100000)
    //  break;
    try {
      bytes = multi.Read(event);
    } catch(const std::out_of_range &oor) {
       run =false;
       break;
    }
    if(bytes<0) {
       run =false;
       break;
    }  
 
    if(event->GetEventType()==8)
       continue;
    if(abs(event->GetTimeStamp()-LastTime)>500) {  // 5 us build time.
      //printf( DRED "build "  RESET_COLOR "\n");
      phoswall->FindWeightedPosition();
      gretina->BuildAddBack();
     
      //phoswall->GetPosition().Print();

      tree->Fill();      

      gretina->Clear();
      phoswall->Clear();
      labr->Clear();
    }

    switch(event->GetEventType()) {
      case 1: {
        //G2Fragment frag(*event);
        TGretinaHit hit(*event);
        gretina->AddGretinaHit(hit);
        gretcounter++;
      }
      break;
      case 17: {
        PWFragment frag(*event);
        phoswall->AddPWHit(frag);
        phoscounter++;
      }
      break;
      case 18: {
        LaBrFragment frag(event);
        //frag.Print();
        labr->Set(frag);
        labrcounter++;
      }
      break;
    };  

    LastTime = event->GetTimeStamp();
    LastType = event->GetEventType();


    if((loopcounter%100000)==0)
       multi.Print();

  }

  ///build anything left in the buffer before we close.
  phoswall->FindWeightedPosition();
  gretina->BuildAddBack();
  tree->Fill();      
  gretina->Clear();
  phoswall->Clear();
  labr->Clear();
  


  outfile = tree->GetCurrentFile();
  outfile->Write();
  outfile->Close();


  return 0;

}









