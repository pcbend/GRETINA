#ifndef G2FRAGMENT_H 
#define G2FRAGMENT_H 


#include<vector>

#include <TObject.h>
#include <TGEBEvent.h>

class G2Segment;
//class TGEBBankType1;

class G2Fragment : public TObject {

  public:
    G2Fragment();
    G2Fragment(const G2Fragment&);
    G2Fragment(const TGEBEvent::TGEBBankType1&);
    G2Fragment(const TGEBEvent&);

    ~G2Fragment();
    G2Fragment& operator=(const G2Fragment&);
    G2Fragment& operator=(const TGEBEvent::TGEBBankType1&);
    G2Fragment& operator=(const TGEBEvent&);

    void Copy(const G2Fragment&);
    void Copy(const TGEBEvent::TGEBBankType1&);
    void Copy(const TGEBEvent&);  

    void Clear(Option_t *opt ="");
    void Print(Option_t *opt ="") const;

      
    const inline Int_t   &GetCrystalNumber()          { return fCrystal; }
    const inline Int_t   &GetNumberOfHits()           { return fNumberHits;  }
    const inline Float_t &GetTotalEnergy()            { return fTotalE;  }
    const inline Int_t   &GetCoreCharge(unsigned int &i) { if(i>3) i=3;  return fCoreCharge[i]; }
    const inline Long_t  &GetTimeStamp()              { return fTimeStamp;      }
    const inline Long_t  &GetTriggerTime()            { return fTriggerTime;    }
    const inline Float_t &GetWalkCorrection()         { return fWalkCorrection; }
    const inline Float_t &GetCfd()                    { return fCfd;            }
    const inline Float_t &GetChisq()                  { return fChisq;          }
    const inline Float_t &GetNormChisq()              { return fNormChisq;      }
    const inline Float_t &GetBaseline()               { return fBaseline;       }
    const inline Float_t &GetPrestep()                { return fPrestep;        }
    const inline Float_t &GetPoststep()               { return fPoststep;       }
    const inline Int_t   &GetPad()                    { return fPad;            }

    const inline G2Segment &GetSegment(unsigned int &i) 
                                      { if(i>fSegments.size()) i=fSegments.size()-1; return fSegments.at(i); } 

    const inline G2Segment &Get(unsigned int &i) { return GetSegment(i); }

    const inline int GetSegmentSize() { return fSegments.size(); }

  private:
    Int_t   fCrystal;
    Int_t   fNumberHits;
    Float_t fTotalE;
    Int_t   fCoreCharge[4];
    Long_t  fTimeStamp;
    Long_t  fTriggerTime;
    Float_t fWalkCorrection;
    Float_t fCfd;
    Float_t fChisq;
    Float_t fNormChisq;
    Float_t fBaseline;
    Float_t fPrestep;
    Float_t fPoststep;
    Int_t   fPad;

    std::vector<G2Segment> fSegments;

  ClassDef(G2Fragment,1)
};






#endif 


