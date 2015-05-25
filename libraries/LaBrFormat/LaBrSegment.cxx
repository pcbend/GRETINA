
#include "LaBrSegment.h"

#include "TGEBBanks.h"
#include "TPhosWall.h"

ClassImp(LaBrSegment)

LaBrSegment::LaBrSegment() { Clear(); }

LaBrSegment::~LaBrSegment() { }

void LaBrSegment::Copy(const LaBrSegment &rhs) {
  Clear();
  fCharge   = rhs.fCharge;
  fId       = rhs.fId;
  fTime     = rhs.fTime;

  fPosition  = rhs.fPosition;  

}

LaBrSegment::LaBrSegment(const LaBrSegment &rhs) {
  Copy(rhs);
}


LaBrSegment &LaBrSegment::operator=(const LaBrSegment &rhs) {
  if(&rhs!=this)
    Clear();
  this->Copy(rhs);
  return *this; 
}


void LaBrSegment::SetCharge(const TGEBEvent::TLaBrSeg &rhs) {
  if(fId==-1) {
    fId = rhs.chan_id;
  } else if(fId != rhs.chan_id) {
    printf("\t LaBrSegment::SetCharge  -> trying to set charge for id[%i], does not non match stored id[%i] !\n",rhs.chan_id,fId);
    return; 
  }
  fCharge = rhs.value;
  return;
}

void LaBrSegment::SetTime(const TGEBEvent::TLaBrSeg &rhs) {
  if(fId==-1) {
    fId = rhs.chan_id;
  } else if(fId != rhs.chan_id) {
    printf("\t LaBrSegment::SetTime   -> trying to set timee for id[%i], does not non match stored id[%i] !\n",rhs.chan_id,fId);
    return; 
  }
  fTime = rhs.value;
  return;

}

bool LaBrSegment::IsValid() {
  if((fId     !=-1) &&
     (fCharge !=-1) &&
     (fTime   !=-1) )
     return true;
  return false;
}


void LaBrSegment::Clear(Option_t *opt) {
  fId     = -1;
  fCharge = -1;
  fTime   = -1;
  return;
}

void LaBrSegment::Print(Option_t *opt) const {
  printf(" Id[%02i]\t Charge:\t  %i  Time:\t  %i  \n",fId,fCharge,fTime);
  //printf("Pixel[%03i]\t%i\t%i\t%i\t%i\t| x[%02.02f] y[%02.02f] z[%02.02f] \n"
  //,fPixel,fACharge,fBCharge,fCCharge,fTime,fPosition.X(),fPosition.Y(),fPosition.Z());
  //fPosition  = GetPosition(fPixel);  
}

TVector3 LaBrSegment::FindPosition(int pixel) {
  TVector3 pos(0,0,1); // = //*(TPhosWall::FindWallPosition(pixel));
  //pos.SetXYZ(0,0,1);
  
  return pos;
}


