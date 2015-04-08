
#include <Globals.h>

#include <TGretina.h>
#include <TGretinaHit.h>

#include <G2Fragment.h>
#include <G2Segment.h>


ClassImp(TGretinaHit)


Float_t TGretinaHit::fFirstSegmentValue  = -1.0;   
Float_t TGretinaHit::fSecondSegmentValue = -1.0;   



TGretinaHit::TGretinaHit() { Clear(); }


TGretinaHit::~TGretinaHit() { }

TGretinaHit& TGretinaHit::operator+=(const TGretinaHit& rhs) {
  if(this->fCoreEnergy<fCoreEnergy) {
    this->fTimeStamp      =  rhs.fTimeStamp;
    this->fWalkCorrection =  rhs.fWalkCorrection;
    this->fCorePosition   =  rhs.fCorePosition;
  } else {
    this->fCurrentTime     = rhs.fCurrentTime;
    this->fCorePosition    = rhs.fCorePosition;
  }
  this->fCoreEnergy += rhs.fCoreEnergy;
  return *this;
}

inline TGretinaHit operator+(TGretinaHit lhs,const TGretinaHit& rhs) {
  lhs += rhs;
  return lhs;
}


bool TGretinaHit::CheckAddback(TGretinaHit& rhs) {
  if(!CheckPosition() || !rhs.CheckPosition())
    return false;
  TVector3 res = GetPosition() - rhs.GetPosition();
  double dtime = fabs(GetTime() - rhs.GetTime());
  if((res.Mag()<250.0) && (dtime<20.0)) {
  //printf(DYELLOW);
  //printf("res.Mag() %.02f\tdtime %.02f\t       [1]:%.02f\t[2]:%.02f",res.Mag(),dtime,GetCoreEnergy(),rhs.GetCoreEnergy());
  //printf(RESET_COLOR "\n");
     return true;
  }
  //printf(DRED);
  //printf("res.Mag() %.02f\tdtime %.02f\t       [1]:%.02f\t[2]:%.02f",res.Mag(),dtime,GetCoreEnergy(),rhs.GetCoreEnergy());
  //printf(RESET_COLOR "\n");
  return false;
}

TGretinaHit::TGretinaHit(G2Fragment &frag) { 
  Clear(); 
 
  fTimeStamp      =   frag.GetTimeStamp();
  fWalkCorrection =   frag.GetWalkCorrection();

  fCrystalId      =   frag.GetCrystalNumber();
  fCoreEnergy     =   frag.GetTotalEnergy();
  unsigned int tmp = 0;
  fCoreCharge     =   frag.GetCoreCharge(tmp);

  for(int x=0;x<frag.GetSegmentSize();x++) {
    tmp = (unsigned int)x;
    G2Segment seg = frag.GetSegment(tmp);

    fSegmentNumber.push_back(seg.Number());
    fSegmentEnergy.push_back(seg.SegmentEnergy());
    fInteractionPosition.push_back(TGretina::FindPosition(fCrystalId,
                                                          seg.X(),seg.Y(),seg.Z()));
    if(seg.Energy()>fFirstSegmentValue) {
      if(x>0) {
         fSecondSegmentValue = fFirstSegmentValue;
         fSecondInteraction  = fFirstInteraction;
      }
      fFirstSegmentValue = seg.Energy();
      fFirstInteraction  = x;
    }
    //printf("\t%i\tseg.Enery() : %.02f \n",x,seg.Energy());
    //printf("In loop\n");
  }
  //printf("fFirstInteraction = %i\n",fFirstInteraction);
  //printf("----------------------\n");
  fCorePosition = fInteractionPosition.at(fFirstInteraction);
  fCurrentTime = fTimeStamp - fWalkCorrection;

}

void TGretinaHit::Print(Option_t *opt) { }


void TGretinaHit::Clear(Option_t *opt) {
    fFirstSegmentValue  = -1.0;   
    fSecondSegmentValue = -1.0;  

    fTimeStamp      =   0;
    fWalkCorrection =  -1;

    fCrystalId      =  -1;
    fCoreEnergy     = 0.0;
    fCoreCharge     =   0;

    fFirstInteraction  = 0;
    fSecondInteraction = 0;
 
    fCorePosition.SetXYZ(0,0,1);
    fCurrentTime = 0;


    fSegmentNumber.clear();
    fInteractionPosition.clear();
    fSegmentEnergy.clear();

}





