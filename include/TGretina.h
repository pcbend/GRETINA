#ifndef TGRETINA_H 

#include <TObject.h>

#include <G2Fragment.h>
#include <G2Segment.h>


class TSegHit : public TObject {

  public:
    TSegHit();
    ~TSegHit();

    void Clear(Option_t *opt = "");
    void Print(Option_t *opt = "");

    void  SetHit(G2Segment&);
    Int_t Size() {return fSegPosition.size();}

    inline TVector3 GetSegPosition(const int &i) { return fSegPosition.at(i); }
    inline Float_t  GetSegEnergy(const int &i)   { return fSegEnergy.at(i);   }
    inline Int_t    GetSegNumber(const int &i)   { return fSegNumber.at(i);   }
    inline Float_t  GetSegSegEnergy(const int &i)   { return fSegSegEnergy.at(i);   }


  private:

    std::vector<TVector3> fSegPosition;
    std::vector<Float_t>  fSegEnergy;
    std::vector<Int_t>    fSegNumber;
    std::vector<Float_t>  fSegSegEnergy;


  ClassDef(TSegHit,1);
};


class TGretina : public TObject {
  
  public:
    TGretina();
    ~TGretina();

    void AddGretinaHit(G2Fragment &frag); //{ GretinaHits.push_back(frag); }
    //inline const int NumberOfGretinaHits() { return fCrystal.size(); }//GretinaHits.size(); }

    void Print(Option_t *opt = "");
    void Clear(Option_t *opt = "");

    void SetID(const Int_t &id)                { fCrystalId.push_back(id);       }
    void SetEnergy(const Float_t &eng)         { fTotalE.push_back(eng);         }
    void SetLed(const Float_t &led)            { fLed.push_back(led);            }
    void SetCharge(const Int_t &chg)           { fCoreCharge.push_back(chg);     }
    void SetTimeStamp(const Long_t &tim)       { fTimeStamp.push_back(tim);      }
    void SetTZero(const Float_t &wak)          { fTZero.push_back(wak); }
    void SetPosition(TVector3 &pos)            { fPosition.push_back(pos);       }

    inline Int_t    GetID(const int &i)             { return fCrystalId.at(i);   }
    inline Float_t  GetEnergy(const int &i)         { return fTotalE.at(i);      }
    inline Float_t  GetLed(const int &i)            { return fLed.at(i);         }
    inline Int_t    GetCharge(const int &i)         { return fCoreCharge.at(i);  }
    inline Long_t   GetTimeStamp(const int &i)      { return fTimeStamp.at(i);   }
    inline Float_t  GetTZero(const int &i)          { return fTZero.at(i); }
    inline TVector3 GetPosition(const int &i)       { return fPosition.at(i);    }

    inline TSegHit *GetSegment(const int &i)        { return &fSegments.at(i);   }

    Int_t Size()                 { return fCrystalId.size(); }
    Int_t SegSize(const int &i)  { return fSegments.at(i).Size(); }

  private:
    //std::vector<G2Fragment> GretinaHits;

    std::vector<Int_t>    fCrystalId;
    //std::vector<Int_t>    fNumberHits;
    std::vector<Float_t>  fTotalE;
    std::vector<Int_t>    fCoreCharge; //[4];
    std::vector<Long_t>   fTimeStamp;
    std::vector<Long_t>   fLed; 
    std::vector<Float_t>  fTZero;
    std::vector<TVector3> fPosition;
    //std::vector<Float_t> fCfd;
    //std::vector<Float_t> fChisq;
    //std::vector<Float_t> fNormChisq;
    //std::vector<Float_t> fBaseline;
    //std::vector<Float_t> fPrestep;
    //std::vector<Float_t> fPoststep;
    //std::vector<Int_t>   fPad;
 
    //std::vector<std::vector G2Segment> segments 
    std::vector<TSegHit> fSegments;

  ClassDef(TGretina,1);
};





#endif
  
  
  
  
  
  
  
  
