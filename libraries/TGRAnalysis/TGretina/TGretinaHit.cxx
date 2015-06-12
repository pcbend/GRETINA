
#include <Globals.h>

#include <TGretina.h>
#include <TGretinaHit.h>

#include <G2Fragment.h>
#include <G2Segment.h>


ClassImp(TGretinaHit)


Float_t TGretinaHit::fFirstSegmentValue  = -1.0;   
Float_t TGretinaHit::fSecondSegmentValue = -1.0;   

TVector3 TGretinaHit::beam_direction;

TGretinaHit::TGretinaHit() { beam_direction.SetXYZ(0,0,1); Clear(); }


TGretinaHit::~TGretinaHit() { }

TGretinaHit& TGretinaHit::operator+=(const TGretinaHit& rhs) {
  if(this->fCoreEnergy<fCoreEnergy) {
    this->fTimeStamp      =  rhs.fTimeStamp;
    this->fWalkCorrection =  rhs.fWalkCorrection;
    this->fCorePosition   =  rhs.fCorePosition;
  } else {
    this->fAddress         = rhs.fAddress;
    //this->fCurrentTime     = rhs.fCurrentTime;
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

TGretinaHit::TGretinaHit(TGEBEvent &event) {
  Copy(*((TGEBEvent::TGEBBankType1*)(event.GetData())));
}

TGretinaHit::TGretinaHit(G2Fragment &frag) { 
  beam_direction.SetXYZ(0,0,1);  
  Clear(); 


  fAddress        =   frag.GetAddress(); 
  fTimeStamp      =   frag.GetTimeStamp();
  fWalkCorrection =   frag.GetWalkCorrection();

  fCrystalId      =   frag.GetCrystalNumber();
  fCoreEnergy     =   frag.GetTotalEnergy();
  unsigned int tmp = 0;
  fCoreCharge     =   frag.GetCoreCharge(tmp);
  
  fNumberOfSegments = frag.GetSegmentSize();  

  for(int x=0;x<frag.GetSegmentSize();x++) {
    tmp = (unsigned int)x;
    G2Segment seg = frag.GetSegment(tmp);

    //fSegmentNumber.push_back(seg.Number());
    //fSegmentEnergy.push_back(fabs(seg.SegmentEnergy()));
    //fInteractionPosition.push_back(TGretina::FindPosition(fCrystalId,
    //                                                      seg.X(),seg.Y(),seg.Z()));
    fSegmentNumber[x]       = seg.Number();
    fInteractionPosition[x] = TGretina::FindPosition(fCrystalId,seg.X(),seg.Y(),seg.Z());
    fSegmentEnergy[x]       = seg.SegmentEnergy();                                                                            
    float tmpsegenergy = seg.Energy();
    if(tmpsegenergy<0.01)
      tmpsegenergy = 0.01;
    if(tmpsegenergy>fFirstSegmentValue) {
      if(x>0) {
         fSecondSegmentValue = fFirstSegmentValue;
         fSecondInteraction  = fFirstInteraction;
      }
      fFirstSegmentValue = tmpsegenergy;
      fFirstInteraction  = x;
    }
    //printf("\t%i\tseg.Enery() : %.02f \n",x,seg.Energy());
    //printf("In loop\n");
  }
  //printf("fFirstInteraction = %i\n",fFirstInteraction);
  //printf("----------------------\n");
  if(fFirstInteraction>-1) {
    fCorePosition = fInteractionPosition[fFirstInteraction];
    //fCurrentTime = fTimeStamp - fWalkCorrection;
  }
}



void TGretinaHit::Copy(const TGEBEvent::TGEBBankType1 &rhs) {
  Clear();

  fAddress        = ((rhs.type<<24) + (rhs.crystal_id<<8) + 36);
  fTimeStamp      = rhs.timestamp;
  fWalkCorrection = rhs.t0; 
  fCrystalId      = rhs.crystal_id;
  fCoreEnergy     = rhs.tot_e;
  
  int tmp = 0;
  fCoreCharge     = rhs.core_e[tmp];

  fNumberOfSegments = rhs.num; //fNumberHits;  

  for(int x=0;x<fNumberOfSegments;x++) {
    fSegmentNumber[x]       = rhs.intpts[x].seg;
    fInteractionPosition[x] = TGretina::FindPosition(fCrystalId,rhs.intpts[x].x,rhs.intpts[x].y,rhs.intpts[x].z);
    fSegmentEnergy[x]       = rhs.intpts[x].seg_ener;

    if(fSegmentEnergy[x]>fFirstSegmentValue) {
       if(fFirstSegmentValue >fSecondSegmentValue) {
         fSecondInteraction = fFirstInteraction;
         fSecondSegmentValue = fFirstSegmentValue;
       }
       fFirstInteraction  = x;
       fFirstSegmentValue = fSegmentEnergy[x];
    }     
  }
}

void TGretinaHit::Copy(const TGretinaHit &rhs) {
  Clear();
  fAddress        = rhs.fAddress; 
  fTimeStamp      = rhs.fTimeStamp;
  fWalkCorrection = rhs.fWalkCorrection; 
  fCrystalId      = rhs.fCrystalId;
  fCoreEnergy     = rhs.fCoreEnergy;
  
  fCoreCharge     = rhs.fCoreCharge;

  fNumberOfSegments = rhs.fNumberOfSegments; //fNumberHits;  

  fFirstInteraction  = rhs.fFirstInteraction;
  fSecondInteraction = rhs.fSecondInteraction;

  for(int x=0;x<fNumberOfSegments;x++) {
    fSegmentNumber[x]       = rhs.fSegmentNumber[x];
    fInteractionPosition[x] = rhs.fInteractionPosition[x];
    fSegmentEnergy[x]       = rhs.fSegmentEnergy[x];
  }

}


void TGretinaHit::Print(Option_t *opt) { }


void TGretinaHit::Clear(Option_t *opt) {
    fFirstSegmentValue  = -1.0;   
    fSecondSegmentValue = -1.0;  

    fAddress        =  -1;
    fTimeStamp      =   0;
    fWalkCorrection =  -1;

    fCrystalId      =  -1;
    fCoreEnergy     = 0.0;
    fCoreCharge     =   0;

    fFirstInteraction  = -1;
    fSecondInteraction = -1;
 
    fCorePosition.SetXYZ(0,0,1);
    //fCurrentTime = 0;

    for(int x=0;x<MAXHPGESEGMENTS;x++) {
      fSegmentNumber[x]       = -1;
      fInteractionPosition[x].SetXYZ(0,0,1);
      fSegmentEnergy[x]    = -1;
    }
}





