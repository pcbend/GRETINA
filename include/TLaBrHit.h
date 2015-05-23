#ifndef TLABRHIT_H
#define TLABRHIT_H

#include <TObject.h>
#include <Rtypes.h>
#include <TVector3.h>
#include <TMath.h>

#include <cmath>

class LaBrFragment;
class LaBrSegment;

class TLaBrHit : public TObject {

  public:
    TLaBrHit();
    TLaBrHit(LaBrSegment&);
    ~TLaBrHit();

    inline Double_t GetTime()       { return (double)fTimeStamp; }
    inline Int_t    GetCrystalId()  { return fId;  }
    inline Float_t  GetTDC()        { return fTdc-fRefTdc; }
    inline Float_t  GetEnergy()     { return fEnergy; }
    inline Float_t  GetCharge()     { return fCharge; }


    void  Print(Option_t *opt="") const;
    void  Clear(Option_t *opt="");

    TVector3 &GetPosition()         { return fPosition;    }
 
    //void SetPosition(TVector3 &vec) { fCorePosition = vec; } 
    void SetTimeStamp(const Long_t ts)  { fTimeStamp = ts; }  
    void SetTDCRef(const Int_t tdc_ref) { fRefTdc = tdc_ref; }  

    bool IsValid();

  private:
    Long_t  fTimeStamp;
    Int_t   fRefTdc;    
    Int_t   fTdc;    

    Int_t   fId;
    Float_t fEnergy;
    Int_t   fCharge;

    TVector3 fPosition;

  ClassDef(TLaBrHit,1)

};


#endif
