#ifndef PWSEGMENT_H
#define PWSEGMENT_H

#include <TVector3.h>
#include <TObject.h>

#include <TGEBEvent.h>

//class TGEBGeSegType1;

class PWSegment : public TObject {

  public:
    PWSegment();
    PWSegment(const PWSegment&);
    PWSegment(const TGEBEvent::TPWHit&);
    ~PWSegment();
    PWSegment& operator=(const PWSegment&);
    PWSegment& operator=(const TGEBEvent::TPWHit&);

    void Copy(const PWSegment&);
    void Copy(const TGEBEvent::TPWHit&);

    void Clear(Option_t *opt ="");
    void Print(Option_t *opt ="");

    inline Double_t X() { return fPosition.X(); }
    inline Double_t Y() { return fPosition.Y(); }
    inline Double_t Z() { return fPosition.Z(); }

    inline Double_t Mag()   { return fPosition.Mag();   }
    inline Double_t Theta() { return fPosition.Theta(); }
    inline Double_t Phi()   { return fPosition.Phi();   }

    inline Short_t  Pixel()  { return fPixel;   }
    inline Short_t  A()      { return fACharge; }
    inline Short_t  B()      { return fBCharge; }
    inline Short_t  C()      { return fCCharge; }
    inline Short_t  Time()   { return fTime;    }

    inline TVector3 Position() { return fPosition; }

    static TVector3 GetPosition(int PixelId);

  private:
    TVector3 fPosition;
    Short_t  fACharge;
    Short_t  fBCharge;
    Short_t  fCCharge;
    Short_t  fPixel;
    Short_t  fTime;

  ClassDef(PWSegment,1)

};

#endif

