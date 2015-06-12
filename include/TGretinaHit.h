#ifndef TGRETINAHIT_H
#define TGRETINAHIT_H

#include <TObject.h>
#include <Rtypes.h>
#include <TVector3.h>
#include <TMath.h>

#include <cmath>

#include "TGEBEvent.h"

#define MAXHPGESEGMENTS 32

class G2Fragment;

class TGretinaHit : public TObject {

  public:
    TGretinaHit();
    TGretinaHit(G2Fragment&);
    TGretinaHit(TGEBEvent&);
    ~TGretinaHit();

    void Copy(const TGEBEvent::TGEBBankType1&);
    void Copy(const TGretinaHit&);


    inline Double_t GetTime()       { return (double)fTimeStamp - (double)fWalkCorrection; }
    inline Int_t    GetAddress()    { return fAddress;    }
    inline Int_t    GetCrystalId()  { return fCrystalId;  }
    inline Float_t  GetCoreEnergy() { return fCoreEnergy; }
    inline Float_t  GetCoreCharge() { return fCoreCharge; }

    void  Print(Option_t *opt="");
    void  Clear(Option_t *opt="");
    const Int_t Size()  { return fNumberOfSegments; }//fSegmentNumber.size(); }

    TVector3 &GetPosition() { 
       //printf("fFirstInteraction =  %i |  fInteractionPosition.size() = %i \n",
       //        fFirstInteraction,fInteractionPosition.size());
       //return fInteractionPosition.at(fFirstInteraction);
       return fCorePosition;
    }
 
    inline double GetPhi() { double phi = fCorePosition.Phi(); 
                             if(phi<0) {phi = phi+TMath::TwoPi();} 
                             if(phi>TMath::TwoPi()) { phi=phi-TMath::TwoPi(); }
                             return phi; }
    inline double GetTheta() { return fCorePosition.Theta(); }
    inline double GetPhiDeg() { return GetPhi()*TMath::RadToDeg(); }
    inline double GetThetaDeg() { return GetTheta()*TMath::RadToDeg(); }


    inline bool CheckPosition() { /*printf("fNumberOfSegments = %i\n",fNumberOfSegments);*/ if(fNumberOfSegments>0) return true; return false; }
    bool CheckAddback(TGretinaHit&);

    TGretinaHit& operator+=(const TGretinaHit&);


    inline double GetDoppler(double beta,TVector3 *vec=0) {
      if(Size()<1)
         return 0.0;
      bool madevec = false;
      //int gid = fCrystalId;
      if(vec==0) {
         vec = &beam_direction;
      }
      double tmp = 0;
      //if(beta != 0.00) {
      double gamma = 1/(sqrt(1-pow(beta,2)));
      tmp = fCoreEnergy*gamma *(1 - beta*TMath::Cos(GetPosition().Angle(*vec)));
      //}
      return tmp;
   }

   
   inline Int_t   GetFirstInteraction()   { return fFirstInteraction;     }
   inline Int_t   GetSecondInteraction() { return fSecondInteraction;    }
   inline Int_t   NumberOfSegments() { return fNumberOfSegments; }
   inline Int_t   GetSegmentId(const int &i)  { return fSegmentNumber[i]; }
   inline Float_t GetSegmentEng(const int &i) { return fSegmentEnergy[i]; }
   inline TVector3 GetInteractionPosition(const int &i) { return fInteractionPosition[i]; }

   //void SetPosition(TVector3 &vec) { fCorePosition = vec; } 

  private:
    Long_t  fTimeStamp;
    Float_t fWalkCorrection;

    Int_t   fAddress;
    Int_t   fCrystalId;
    Float_t fCoreEnergy;
    Int_t   fCoreCharge;

    TVector3 fCorePosition;
    //Float_t  fCurrentTime;

    Int_t   fFirstInteraction;
    Int_t   fSecondInteraction;
 
    static  Float_t fFirstSegmentValue;    
    static  Float_t fSecondSegmentValue;   

    Int_t   fNumberOfSegments;

    //std::vector<Int_t>    fSegmentAddress;
    //std::vector<Int_t>    fSegmentNumber;
    //std::vector<TVector3> fInteractionPosition;
    //std::vector<Float_t>  fSegmentEnergy;

    Int_t    fSegmentNumber[MAXHPGESEGMENTS];
    TVector3 fInteractionPosition[MAXHPGESEGMENTS];
    Float_t  fSegmentEnergy[MAXHPGESEGMENTS];

    static TVector3 beam_direction; //!

  ClassDef(TGretinaHit,3)

};


#endif
