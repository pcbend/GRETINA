//g++ MakeTree.cxx `grut-config --cflags --libs --root` 

#include <cstdio>
#include <string>

#include <TGEBMultiFile.h>
#include <TGEBFile.h>
#include <TGEBEvent.h>

#include <Globals.h>

#include <Rtypes.h>

#include <TGretina.h>
#include <TPhosWall.h>

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

  TGEBEvent *event = multi.Read();
  if(!event) {
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

  if(outfilename.length()==0) {
     printf("\tno outfile file specified, using default: myoutput.root\n");
     outfilename.assign("myoutput.root");
  }


  if(outfilename.find(".root")==std::string::npos)
     outfilename.append(".root");

  TFile *outfile = new TFile(outfilename.c_str(),"recreate");
  TTree *tree = new TTree("Data","Data");
  TTree::SetMaxTreeSize((Long_t)1*(Long_t)1024*(Long_t)1024*(Long_t)1024);
  tree->Bronch("TGretina","TGretina",&gretina);
  tree->Bronch("TPhosWall","TPhosWall",&phoswall);

  bool run = true;
  int loopcounter=0;

  while(run) {
    loopcounter++;
    event = multi.Read();
    if(!event) {
       run =false;
       break;
    }   
    if(event->GetEventType()==8)
       continue;
    if(abs(event->GetTimeStamp()-LastTime)>500) {  // 5 us build time.
      phoswall->FindWeightedPosition();
      gretina->BuildAddBack();
     
      phoswall->GetPosition()->Print();

      tree->Fill();      

      gretina->Clear();
      phoswall->Clear();
    }

    switch(event->GetEventType()) {
      case 1: {
        G2Fragment frag(*event);
        TGretinaHit hit(frag);
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
    };  

    LastTime = event->GetTimeStamp();
    LastType = event->GetEventType();


    if((loopcounter%100000)==0)
       multi.Print();

  }
  outfile = tree->GetCurrentFile();
  outfile->Write();
  outfile->Close();


  return 0;

}









