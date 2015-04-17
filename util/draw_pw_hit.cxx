
#include <cstdio>
#include <string>

#include <TGEBMultiFile.h>
#include <TGEBEvent.h>

#include <TGretina.h>
#include <TPhosWall.h>
#include <PWFragment.h>

#include <TROOT.h>
#include <TApplication.h>
#include <TCanvas.h>


void DrawPwHit(const char *fname) {
  TGEBMultiFile multi; 
  multi.Add(fname);

  TGEBEvent *event = multi.Read();
  if(!event) {
     printf("Error reading event from %s\n",fname);
     return;
  }
  Long_t LastTime = event->GetTimeStamp();
  Int_t  LastType = event->GetEventType();

  TGretina  *gretina  = new TGretina;
  TPhosWall *phoswall = new TPhosWall;

  int phoscounter = 0;
  int notphoscounter = 0;

  TApplication *app = new TApplication("DrawPwHit",0,0);

  bool run = true;
  while(run) {
  
      event = multi.Read();
      if(!event)
         break;
      if(event->GetEventType()!=17) {
         notphoscounter++;
         if(notphoscounter>10000) {
            printf("Are you sure %s contains PhosWall events??\n",fname);
            break;
         }
         continue;
      }

      if(abs(event->GetTimeStamp()-LastTime)>500) {
         phoswall->FindWeightedPosition();
         if(phoswall->Multiplicity()>0) {
           phoswall->Print();
           TCanvas *c;
           if(!gPad)
              c = new TCanvas();
           else 
              c = gPad->GetCanvas();
           c->Clear();
           c->Divide(2,1);
           c->cd(1);
           phoswall->DrawXY();
           gPad->AddExec("DrawPWExec",".x DrawPWExec.C");
           c->cd(2);
           phoswall->Draw("DrawPWExec");
           app->Run(true);
           //phoswall->Draw("");
           //gPad->AddExec("myexec",".x myexec.C");
           //new TCanvas();
           //phoswall->DrawXY("");
           //gPad->AddExec("myexec",".x myexec.C");
           //app->Run(true);
           //gPad->GetCanvas()->Close();
         }
      }

      switch(event->GetEventType()) {
        case 17: {
            TGEBEvent::TPWBank *bank = (TGEBEvent::TPWBank*)event->GetData();
            PWFragment frag(*event);
            phoswall->AddPWHit(frag);
            phoscounter++;
          }
          phoscounter++;
          break;
        default:
          break;

      };
  }
  printf("end of file.\n");
  return;

}







#ifndef __CINT__

int main(int argc,char**argv) {
  if(argc !=2) {
    printf("Usage: DrawPwHit <filename.cvt>\n");
    return 1;
  }
  std::string new_path = getenv("GEBSYS");
  new_path.append("/util");
  gROOT->SetMacroPath(new_path.c_str());
  DrawPwHit(argv[1]);
  return 0;
}

#endif
