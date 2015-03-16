#include "PWFragment.h"
#include "PWSegment.h"


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

void PWFragment::Print(Option_t *opt) { }




