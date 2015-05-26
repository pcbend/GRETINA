
#include "G2Segment.h"

#include "TGEBBanks.h"

ClassImp(G2Segment)

G2Segment::G2Segment() { Clear(); }

G2Segment::~G2Segment() { }

void G2Segment::Copy(const G2Segment &rhs) {
  fPosition      = rhs.fPosition;
  fEnergy        = rhs.fEnergy;
  fSegmentNumber = rhs.fSegmentNumber;
  fSegmentEnergy = rhs.fSegmentEnergy;
}

void G2Segment::Copy(const TGEBEvent::TGEBGeSegType1 &rhs) {
  fPosition.SetXYZ(rhs.x,rhs.y,rhs.z);
  fEnergy        = rhs.e;
  fSegmentNumber = rhs.seg;
  fSegmentEnergy = rhs.seg_ener;
}

G2Segment::G2Segment(const G2Segment &rhs) {
  Copy(rhs);
}

G2Segment::G2Segment(const TGEBEvent::TGEBGeSegType1 &rhs) {
  Copy(rhs);
}

G2Segment &G2Segment::operator=(const G2Segment &rhs) {
  if(&rhs!=this)
    Clear();
  this->Copy(rhs);
  return *this; 
}
G2Segment &G2Segment::operator=(const TGEBEvent::TGEBGeSegType1 &rhs) {
  Clear();
  this->Copy(rhs);
  return *this;
}

void G2Segment::Clear(Option_t *opt) { }

void G2Segment::Print(Option_t *opt) { }




