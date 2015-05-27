

#include <TLaBr.h>
#include <LaBrFragment.h>

#include <TChannel.h>

ClassImp(TLaBr)


TLaBr::TLaBr() { 
   Clear(); 
}

TLaBr::TLaBr(LaBrFragment &frag) { 
   Clear(); 
   for(int x=0;x<frag.Size();x++) {
     TLaBrHit hit(frag.Get(x));
     hit.SetTimeStamp(frag.GetTimeStamp());
     hit.SetTDCRef(frag.GetTDCRef());
     if(hit.IsValid())
       labr_hits.push_back(hit);
   }
}

TLaBr::~TLaBr() {  }

void TLaBr::Set(LaBrFragment &frag) {
   Clear(); 
   TChannel *chan = 0;
   for(int x=0;x<frag.Size();x++) {
     TLaBrHit hit(frag.Get(x));
     hit.SetAddress(frag.GetAddress(x));
     chan=TChannel::GetChannel(hit.GetAddress());
     if(chan)
       hit.SetEnergy(chan->CalibrateENG(hit.GetCharge()));
     hit.SetTimeStamp(frag.GetTimeStamp());
     hit.SetTDCRef(frag.GetTDCRef());
     if(hit.IsValid())
       labr_hits.push_back(hit);
   }
}

void TLaBr::Print(Option_t *opt) const { }

void TLaBr::Clear(Option_t *opt) {
  labr_hits.clear();
}


TVector3 TLaBr::FindPosition(int cryId) {
   TVector3 pos;
   pos.SetXYZ(0,0,1);
   return pos;
}
