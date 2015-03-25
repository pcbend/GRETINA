

#include <TGretina.h>


ClassImp(TSegHit)

TSegHit::TSegHit() { }

TSegHit::~TSegHit() { }


void TSegHit::SetHit(G2Segment &seg) {
  fSegPosition.push_back(seg.Position());
  fSegEnergy.push_back(seg.Energy());
  fSegNumber.push_back(seg.Number());
  fSegSegEnergy.push_back(seg.SegmentEnergy());
}


void TSegHit::Print(Option_t *opt) {  }


void TSegHit::Clear(Option_t *opt) {
  fSegPosition.clear();
  fSegEnergy.clear();;
  fSegNumber.clear();;
  fSegSegEnergy.clear();;
}




ClassImp(TGretina)


TGretina::TGretina() { Clear(); }

TGretina::~TGretina() {  }

void TGretina::AddGretinaHit(G2Fragment &frag) {

  SetID(frag.GetCrystalNumber());            
  SetEnergy(frag.GetTotalEnergy());        
  //  printf("cfd = %f\n",frag.GetCfd());
  unsigned int temp = 3;
  SetCharge(frag.GetCoreCharge(temp));        
  SetTimeStamp(frag.GetTimeStamp());     
  SetLed(frag.GetTriggerTime());   
  SetTZero(frag.GetWalkCorrection());
  //SetPosition(frag.GetPosition());      

}



void TGretina::Print(Option_t *opt) { }

void TGretina::Clear(Option_t *opt) {

  fCrystalId.clear();
  fTotalE.clear();
  fCoreCharge.clear(); //[4];
  fTimeStamp.clear();
  fLed.clear();
  fTZero.clear();
  fPosition.clear();

  //for(int x=0;x<SegSize();x++)
  fSegments.clear();
  
}



TVector3 TGretina::FindPosition(int cryId,Float_t localX,Float_t localY,Float_t localZ) { }










