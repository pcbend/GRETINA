
#include "PWSegment.h"

#include "TGEBBanks.h"

ClassImp(PWSegment)

PWSegment::PWSegment() { Clear(); }

PWSegment::~PWSegment() { }

void PWSegment::Copy(const PWSegment &rhs) {

  fACharge   = rhs.fACharge;
  fBCharge   = rhs.fBCharge;
  fCCharge   = rhs.fCCharge;
  fPixel     = rhs.fPixel;
  fTime      = rhs.fTime;

  fPosition  = rhs.fPosition;  

}

void PWSegment::Copy(const TGEBEvent::TPWHit &rhs) {

  fACharge   = rhs.data_a;
  fBCharge   = rhs.data_b;
  fCCharge   = rhs.data_c;
  fPixel     = rhs.pix_id;
  fTime      = rhs.time;

  fPosition  = GetPosition(fPixel);  
}

PWSegment::PWSegment(const PWSegment &rhs) {
  Copy(rhs);
}

PWSegment::PWSegment(const TGEBEvent::TPWHit &rhs) {
  Copy(rhs);
}

PWSegment &PWSegment::operator=(const PWSegment &rhs) {
  if(&rhs!=this)
    Clear();
  this->Copy(rhs);
  return *this; 
}
PWSegment &PWSegment::operator=(const TGEBEvent::TPWHit &rhs) {
  Clear();
  this->Copy(rhs);
  return *this;
}

void PWSegment::Clear(Option_t *opt) { }

void PWSegment::Print(Option_t *opt) { }

TVector3 PWSegment::GetPosition(int pixel) {
  TVector3 pos;
  pos.SetXYZ(0,0,1);
  return pos;
}


