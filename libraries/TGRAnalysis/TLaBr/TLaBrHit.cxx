
#include <Globals.h>

#include <TLaBr.h>
#include <TLaBrHit.h>

#include <LaBrSegment.h>


ClassImp(TLaBrHit)


TLaBrHit::TLaBrHit() { Clear(); }

TLaBrHit::~TLaBrHit() { }


TLaBrHit::TLaBrHit(LaBrSegment &seg) { 
   Clear(); 

   fId         =   seg.ID();
   fEnergy     =   0.0;
   fCharge     =   seg.Charge();
   fTdc        =   seg.Time();

   //fTimeStamp  = -1
   //fRefTdc     = -1;
    
}

bool TLaBrHit::IsValid() {
  if((fId     !=-1) &&
     (fCharge !=-1) &&
     (fTdc    !=-1) )
     return true;
  return false;

}


void TLaBrHit::Print(Option_t *opt) const { }


void TLaBrHit::Clear(Option_t *opt) {

    fTimeStamp  =   0;
    fAddress    =  -1;
    fId         =  -1;
    fEnergy     = 0.0;
    fCharge     =   0;

    fTdc        = -1;
    fRefTdc     = -1;
    fPosition.SetXYZ(0,0,1);
}





