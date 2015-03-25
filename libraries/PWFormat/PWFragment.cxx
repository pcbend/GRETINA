#include "PWFragment.h"
#include "PWSegment.h"

#include "TPhosWall.h"

#include "Globals.h"
#include "TH2.h"

//#include "TGEBBanks.h"

ClassImp(PWFragment)

PWFragment::PWFragment() { Clear(); }

PWFragment::~PWFragment() { }

void PWFragment::Copy(const PWFragment &rhs) {

  Clear();  
  fNumberHits      =  rhs.fNumberHits;      
  fSegments        =  rhs.fSegments;

  fTimeStamp       =  rhs.fTimeStamp;
  fLargestHit      =  rhs.fLargestHit;

}

void PWFragment::Copy(const TGEBEvent::TPWBank &rhs) {
  // doesn't do anything ....
  // I don't know how to make this 
  // work for this data structure...
}

void PWFragment::Copy(const TGEBEvent &event) {
  int a=0;
  fTimeStamp = event.GetTimeStamp();
  for(int x=0;x<event.GetDataSize();x+=sizeof(TGEBEvent::TPWHit)) {
    PWSegment seg = (*((TGEBEvent::TPWHit*)(event.GetData()+x)));
    if(seg.A()>a) {
      a = seg.A();
      fLargestHit = x/sizeof(TGEBEvent::TPWHit);
    }
    fSegments.push_back(seg);
  }
  fNumberHits = fSegments.size();
}

PWFragment::PWFragment(const PWFragment &rhs) {
  Copy(rhs);
}

PWFragment::PWFragment(const TGEBEvent::TPWBank &rhs) {
  Copy(rhs);
}

PWFragment &PWFragment::operator=(const PWFragment &rhs) {
  this->Copy(rhs);
  return *this; 
}
PWFragment &PWFragment::operator=(const TGEBEvent::TPWBank&rhs) {
  this->Copy(rhs);
  return *this;
}

PWFragment::PWFragment(const TGEBEvent &rhs) {
  Copy(rhs);
}

PWFragment &PWFragment::operator=(const TGEBEvent &rhs) {
  this->Copy(rhs);
}



void PWFragment::Clear(Option_t *opt) {
  fNumberHits      =    0; 
  fTimeStamp       =   -1;
  fLargestHit      =    0;
  fSegments.clear();
}

void PWFragment::Print(Option_t *opt) {
   printf("PWFragment at %lu\n",fTimeStamp);
   printf("Found %i hits.\n",fNumberHits);
   for(int x=0;x<fNumberHits;x++) {
      if(x==fLargestHit)
         printf(DYELLOW);
      else 
         printf(BLUE);
      printf("\t"); fSegments.at(x).Print(); 
      printf(RESET_COLOR);
   }
}


void PWFragment::Draw(Option_t *opt) {
  /*
  bool newcanvas = true;
  if(gPad) { 
     if(!gPad->IsEditable()) 
        new canvas = true;
     else {
        if (TestBit(kCanDelete)) 
          gPad->GetListOfPrimitives()->Remove(this);
        gPad->Clear();
        newcanvas = false;
     }
  }
  if(newcanvas)
     gROOT->MakeDefCanvas();
  */
  TH2F hitpattern("hitpattern","PWFragmet XY Hit Pattern",
                   116,-58,58,116,-58,58);

  for(int i=0;i<fNumberHits;i++) {
     hitpattern.Fill(fSegments.at(i).Position().X(),
                     fSegments.at(i).Position().Y(),
                     fSegments.at(i).A());
  }
  hitpattern.Draw("colz");

}


TH2F PWFragment::MakeHitPattern(Option_t *opt) {
  TH2F hitpattern("hitpattern","PWFragmet XY Hit Pattern",
                   116,-58,58,116,-58,58);

  for(int i=0;i<256;i++) {
    hitpattern.Fill(TPhosWall::FindWallPosition(i)->X(),
                    TPhosWall::FindWallPosition(i)->Y(),
                    1);
  }

  for(int i=0;i<fNumberHits;i++) {
     hitpattern.Fill(fSegments.at(i).Position().X(),
                     fSegments.at(i).Position().Y(),
                     fSegments.at(i).A());
  }
  return hitpattern;
}


void PWFragment::DrawSimpleHitPattern(Option_t *opt) {
  TCanvas *c = new TCanvas;
  c->Divide(2,2);
  TH2F hit[4];
  hit[0] = TH2F("hit0","PWFragmet XY Hit Pattern, detector 1 ",
              10,0,10,10,0,10);
  hit[1] = TH2F("hit1","PWFragmet XY Hit Pattern, detector 2 ",
              10,0,10,10,0,10);
  hit[2] = TH2F("hit2","PWFragmet XY Hit Pattern, detector 3 ",
              10,0,10,10,0,10);
  hit[3] = TH2F("hit3","PWFragmet XY Hit Pattern, detector 4 ",
              10,0,10,10,0,10);



  for(int i=0;i<64;i++) {
     //printf("[%02i]  i%8 = %i, i/8 = %i\n",i,i%8,i/8);
     //hit[0].Fill(i%8,i/8,1);
     //hit[1].Fill(i%8,i/8,1);
     //hit[2].Fill(i%8,i/8,1);
     //hit[3].Fill(i%8,i/8,1);
  }

  

  //printf( " i am here.\n");

  for(int i=0;i<fNumberHits;i++) {
     if((fSegments.at(i).Pixel()>-1) && (fSegments.at(i).Pixel() < 256)) {
        //printf("fSegments.at(i).Pixel()    = %i\n",fSegments.at(i).Pixel());
        //printf("fSegments.at(i).Pixel()/64 = %i\n",fSegments.at(i).Pixel()/64);
        //printf("fSegments.at(i).Pixel()%8  = %i\n",fSegments.at(i).Pixel()%8);
        //printf("fSegments.at(i).Pixel()/8  = %i\n",fSegments.at(i).Pixel()/8);
        //printf("fSegments.at(i).A()        = %i\n",fSegments.at(i).A());
        
        hit[fSegments.at(i).Pixel()/64].Fill(fSegments.at(i).Pixel()%8,
                                             fSegments.at(i).Pixel()/8,
                                             fSegments.at(i).A());
     }
  }
  c->cd(1); hit[0].Draw("colz");
  c->cd(2); hit[1].Draw("colz");
  c->cd(3); hit[2].Draw("colz");
  c->cd(4); hit[3].Draw("colz");

  return;
}


