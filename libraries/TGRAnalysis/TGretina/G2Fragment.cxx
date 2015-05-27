#include "G2Fragment.h"
#include "G2Segment.h"


//#include "TGEBBanks.h"

ClassImp(G2Fragment)

G2Fragment::G2Fragment() { Clear(); }

G2Fragment::~G2Fragment() { }

void G2Fragment::Copy(const G2Fragment &rhs) {

  Clear();  
  fType            =  rhs.fType;
  fCrystal         =  rhs.fCrystal;         
  fNumberHits      =  rhs.fNumberHits;      
  fTotalE          =  rhs.fTotalE;          
  fCoreCharge[0]   =  rhs.fCoreCharge[0];   
  fCoreCharge[1]   =  rhs.fCoreCharge[1];   
  fCoreCharge[2]   =  rhs.fCoreCharge[2];   
  fCoreCharge[3]   =  rhs.fCoreCharge[3];   
  fTimeStamp       =  rhs.fTimeStamp;       
  fTriggerTime     =  rhs.fTriggerTime;     
  fWalkCorrection  =  rhs.fWalkCorrection;  
  fCfd             =  rhs.fCfd;             
  fChisq           =  rhs.fChisq;           
  fNormChisq       =  rhs.fNormChisq;       
  fBaseline        =  rhs.fBaseline;        
  fPrestep         =  rhs.fPrestep;         
  fPoststep        =  rhs.fPoststep;        
  fPad             =  rhs.fPad;             
  fSegments        =  rhs.fSegments;

}

void G2Fragment::Copy(const TGEBEvent::TGEBBankType1 &rhs) {
  Clear();
  //std::cout << rhs;
  fType            =  01; //rhs.type;
  fCrystal         =  rhs.crystal_id;;
  fNumberHits      =  rhs.num;
  fTotalE          =  rhs.tot_e;
  fCoreCharge[0]   =  rhs.core_e[0];
  fCoreCharge[1]   =  rhs.core_e[1];
  fCoreCharge[2]   =  rhs.core_e[2];
  fCoreCharge[3]   =  rhs.core_e[3];
  fTimeStamp       =  rhs.timestamp;
  fTriggerTime     =  rhs.trig_time;
  fWalkCorrection  =  rhs.t0;
  fCfd             =  rhs.cfd;
  fChisq           =  rhs.chisq;
  fNormChisq       =  rhs.norm_chisq;
  fBaseline        =  rhs.baseline;
  fPrestep         =  rhs.prestep;
  fPoststep        =  rhs.poststep;
  fPad             =  rhs.pad;
  for(int x=0;x<fNumberHits;x++) 
    fSegments.push_back(G2Segment(rhs.intpts[x]));    
}

void G2Fragment::Copy(const TGEBEvent &rhs) {
  Copy(*((TGEBEvent::TGEBBankType1*)(rhs.GetData())));
}

G2Fragment::G2Fragment(const G2Fragment &rhs) {
  Copy(rhs);
}

G2Fragment::G2Fragment(const TGEBEvent::TGEBBankType1 &rhs) {
  Copy(rhs);
}

G2Fragment &G2Fragment::operator=(const G2Fragment &rhs) {
  this->Copy(rhs);
  return *this; 
}
G2Fragment &G2Fragment::operator=(const TGEBEvent::TGEBBankType1 &rhs) {
  this->Copy(rhs);
  return *this;
}

G2Fragment::G2Fragment(const TGEBEvent &rhs) {
  Copy(rhs);
}

G2Fragment &G2Fragment::operator=(const TGEBEvent &rhs) {
  this->Copy(rhs);
}



void G2Fragment::Clear(Option_t *opt) {

  fType            =   -1;
  fCrystal         =   -1; 
  fNumberHits      =    0; 
  fTotalE          =   -1; 
  fCoreCharge[0]   =   -1;
  fCoreCharge[1]   =   -1;
  fCoreCharge[2]   =   -1;
  fCoreCharge[3]   =   -1;
  fTimeStamp       =   -1;
  fTriggerTime     =   -1;
  fWalkCorrection  =   -1;
  fCfd             =   -1;
  fChisq           =   -1;
  fNormChisq       =   -1;
  fBaseline        =   -1;
  fPrestep         =   -1;
  fPoststep        =   -1;
  fPad             =   -1;
  fSegments.clear();
}

void G2Fragment::Print(Option_t *opt) const {
  //std::cout << *this; 
  return;
}




