

#include <TPhosWall.h>


ClassImp(TPhosWall)


TPhosWall::TPhosWall() { Clear(); }

TPhosWall::~TPhosWall() {  }

void TPhosWall::AddPWHit(PWFragment &frag) {
  Clear();
  fTimeStamp  = frag.GetTimeStamp();
  fLargestHit = frag.GetLargestNumber();

  for(unsigned int x=0;x<frag.GetNumberOfHits();x++) {
    fPosition.push_back(frag.GetSegment(x).Position());
    fACharge.push_back(frag.GetSegment(x).A());
    fBCharge.push_back(frag.GetSegment(x).B());
    fCCharge.push_back(frag.GetSegment(x).C());
    fPixel.push_back(frag.GetSegment(x).Pixel());
    fTime.push_back(frag.GetSegment(x).Time());
  }
}


void TPhosWall::Print(Option_t *opt) { }

void TPhosWall::Clear(Option_t *opt) {

  fTimeStamp  = 0;
  fLargestHit = 0;

  fPosition.clear();
  fACharge.clear();
  fBCharge.clear();
  fCCharge.clear();
  fPixel.clear();
  fTime.clear();

}


