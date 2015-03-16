#ifndef G2SEGMENT_H
#define G2SEGMENT_H

#include <TVector3.h>
#include <TObject.h>

#include <TGEBEvent.h>

//class TGEBGeSegType1;

class G2Segment : public TObject {

  public:
    G2Segment();
    G2Segment(const G2Segment&);
    G2Segment(const TGEBEvent::TGEBGeSegType1&);
    ~G2Segment();
    G2Segment& operator=(const G2Segment&);
    G2Segment& operator=(const TGEBEvent::TGEBGeSegType1&);

    void Copy(const G2Segment&);
    void Copy(const TGEBEvent::TGEBGeSegType1&);

    void Clear(Option_t *opt ="");
    void Print(Option_t *opt ="");

    const inline Double_t X() { return fPosition.X(); }
    const inline Double_t Y() { return fPosition.Y(); }
    const inline Double_t Z() { return fPosition.Z(); }

    const inline Double_t Mag()   { return fPosition.Mag();   }
    const inline Double_t Theta() { return fPosition.Theta(); }
    const inline Double_t Phi()   { return fPosition.Phi();   }

    const inline Int_t   Number()        { return fSegmentNumber; }
    const inline Float_t Energy()        { return fEnergy; }
    const inline Float_t SegmentEnergy() { return fSegmentEnergy; }


  private:
    TVector3 fPosition;
    Float_t  fEnergy;
    Int_t    fSegmentNumber;
    Float_t  fSegmentEnergy;


  ClassDef(G2Segment,1)

};

#endif


