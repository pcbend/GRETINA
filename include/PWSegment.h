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

    const inline Double_t X() { return fPosition.X(); }
    const inline Double_t Y() { return fPosition.Y(); }
    const inline Double_t Z() { return fPosition.Z(); }

    const inline Double_t Mag()   { return fPosition.Mag();   }
    const inline Double_t Theta() { return fPosition.Theta(); }
    const inline Double_t Phi()   { return fPosition.Phi();   }

    const inline Short_t  Pixel()  { return fPixel;   }
    const inline Short_t  A()      { return fACharge; }
    const inline Short_t  B()      { return fBCharge; }
    const inline Short_t  C()      { return fCCharge; }
    const inline Short_t  Time()   { return fTime;    }


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

