#include "LaBrFragment.h"
#include "LaBrSegment.h"

#include "TPhosWall.h"

#include "Globals.h"
#include "TH2.h"

#include "Rtypes.h"

//#include "TGEBBanks.h"

ClassImp(LaBrFragment)

LaBrFragment::LaBrFragment() { Clear(); }

LaBrFragment::~LaBrFragment() { }

void LaBrFragment::Copy(const LaBrFragment &rhs) {

  Clear();  
  fNumberSegments  =  rhs.fNumberSegments;      
  fSegments        =  rhs.fSegments;
  fIdVecMap        =  rhs.fIdVecMap;

  fType            =  rhs.fType;
  fTimeStamp       =  rhs.fTimeStamp;
}


LaBrFragment::LaBrFragment(const LaBrFragment &rhs) {
  Copy(rhs);
}



LaBrFragment::LaBrFragment(const TGEBEvent *event) {  //TGEBEvent::TLaBrBank &rhs) {

   fTimeStamp = event->GetTimeStamp();
   fType      = event->GetEventType();
   const char *data = event->GetData();

   int byte_counter = 0;
   Short_t nqdcs = *((Short_t*)(data+byte_counter)); byte_counter+=2;
   for(int x=0;x<nqdcs;x++) {
     TGEBEvent::TLaBrSeg seg = *((TGEBEvent::TLaBrSeg*)(data+byte_counter)); byte_counter+=sizeof(TGEBEvent::TLaBrSeg);
     LaBrSegment *local = this->FindSegment(seg.chan_id);
     local->SetCharge(seg);
   }
   Short_t ntdcs = *((Short_t*)(data+byte_counter)); byte_counter+=2;
   for(int x=0;x<ntdcs;x++) {
     TGEBEvent::TLaBrSeg seg = *((TGEBEvent::TLaBrSeg*)(data+byte_counter)); byte_counter+=sizeof(TGEBEvent::TLaBrSeg);
     if(seg.chan_id==15) {
       SetTDCRef(seg.value);
     } else {
       LaBrSegment *local = this->FindSegment(seg.chan_id);
       local->SetTime(seg);
     }
   }
}



LaBrFragment &LaBrFragment::operator=(const LaBrFragment &rhs) {
  this->Copy(rhs);
  return *this; 
}


LaBrSegment *LaBrFragment::FindSegment(const int &id) {
  if(fIdVecMap.count(id)==0) {
    LaBrSegment labr_seg;
    labr_seg.SetId(id);
    fSegments.push_back(labr_seg);
    fIdVecMap.insert(std::make_pair(id,fSegments.size()-1));
    return &fSegments.back();
  } else {
    return &fSegments.at(fIdVecMap.at(id));
  }
}


void LaBrFragment::Clear(Option_t *opt) {
  fNumberSegments  =    0; 
  fType            =   -1;
  fTimeStamp       =   -1;
  fSegments.clear();
  fIdVecMap.clear();
}

void LaBrFragment::Print(Option_t *opt) const {  
  printf("LaBr Fragment:\n");
  printf("\tfound %i \"segments\"\n",Size());
  for(int x=0;x<Size();x++) {
    printf("Segment[%i]:\n");
    fSegments.at(x).Print();
  }
  printf("--------------------------\n");
}


