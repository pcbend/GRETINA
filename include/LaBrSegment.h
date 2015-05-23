#ifndef LaBrSEGMENT_H
#define LaBrSEGMENT_H

#include <TVector3.h>
#include <TObject.h>

#include <TGEBEvent.h>

//class TGEBGeSegType1;

class LaBrSegment : public TObject {

  public:
    LaBrSegment();
    LaBrSegment(const LaBrSegment&);
    //LaBrSegment(const TGEBEvent::TLaBrHit&);
    ~LaBrSegment();
    LaBrSegment& operator=(const LaBrSegment&);
    //LaBrSegment& operator=(const TGEBEvent::TLaBrHit&);

    void Copy(const LaBrSegment&);

    void SetId(const int &id) { fId  = id;}
    void SetCharge(const TGEBEvent::TLaBrSeg&);   
    void SetTime(const TGEBEvent::TLaBrSeg&);   
    bool IsValid();

    void Clear(Option_t *opt ="");
    void Print(Option_t *opt ="") const;

    inline Double_t X() { return fPosition.X(); }
    inline Double_t Y() { return fPosition.Y(); }
    inline Double_t Z() { return fPosition.Z(); }

    inline Double_t Mag()   { return fPosition.Mag();   }
    inline Double_t Theta() { return fPosition.Theta(); }
    inline Double_t Phi()   { return fPosition.Phi();   }

    inline Short_t  ID()        { return fId;   }
    inline Short_t  Charge()    { return fCharge; }
    inline Short_t  Time()      { return fTime;    }

    inline TVector3 Position() { return fPosition; }
    static TVector3 FindPosition(int Id);

  private:
    TVector3 fPosition;
    Short_t  fCharge;
    Short_t  fId;
    Short_t  fTime;

  ClassDef(LaBrSegment,1)

};

#endif

