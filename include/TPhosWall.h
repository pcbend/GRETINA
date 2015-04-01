#ifndef TGRETINA_H 

#include <TObject.h>
#include <TVector3.h>

#include <PWFragment.h>
#include <PWSegment.h>

class TPhosWall : public TObject {
  
  public:
    TPhosWall();
    ~TPhosWall();

    void AddPWHit(PWFragment &frag);
    int  Size() { return fTime.size(); }

    const inline Int_t    GetA()        { if(fLargestHit>=fTime.size()) return 0; return (Int_t)fACharge.at(fLargestHit); }
    const inline Int_t    GetB()        { if(fLargestHit>=fTime.size()) return 0; return (Int_t)fBCharge.at(fLargestHit); }
    const inline Int_t    GetC()        { if(fLargestHit>=fTime.size()) return 0; return (Int_t)fCCharge.at(fLargestHit); }
    const inline Int_t    GetTime()     { if(fLargestHit>=fTime.size()) return 0; return (Int_t)fTime.at(fLargestHit);    }
    const inline Int_t    GetLargestNumber() { return fLargestHit; }  
    const inline Int_t    GetPixel()    { if(fLargestHit>=fPixel.size()) return -1; return (Int_t)fPixel.at(fLargestHit); }
    const inline TVector3 GetPosition() { return fWeightedPosition; }

    const inline Int_t    GetASum()   { int chg=0; for(int x=0;x<fACharge.size();x++) {chg+=fACharge.at(x);} return chg;}
    const inline Int_t    GetBSum()   { int chg=0; for(int x=0;x<fBCharge.size();x++) {chg+=fBCharge.at(x);} return chg;}
    const inline Int_t    GetCSum()   { int chg=0; for(int x=0;x<fCCharge.size();x++) {chg+=fCCharge.at(x);} return chg;}


    const inline Int_t    A(const int &i)        { return (Int_t)fACharge.at(i); }
    const inline Int_t    B(const int &i)        { return (Int_t)fBCharge.at(i); }
    const inline Int_t    C(const int &i)        { return (Int_t)fCCharge.at(i); }
    const inline Int_t    Time(const int &i)     { return (Int_t)fTime.at(i);    }
    const inline Int_t    Pixel(const int &i)    { return (Int_t)fPixel.at(i); }

    Int_t    GetASmartSum(float res = 20.0,int threshold=200); 
    Int_t    GetBSmartSum(float res = 20.0,int threshold=200);
    Int_t    GetCSmartSum(float res = 20.0,int threshold=200); 
    //inline const int NumberOfPWHits() { return PhosWallHits.size(); }

    void Print(Option_t *opt = "");
    void Clear(Option_t *opt = "");
    void Draw(Option_t *opt="");
    void DrawXY(Option_t *opt="");

    static TVector3 *FindWallPosition(Int_t &pixel) { if(pixel<0||pixel>256) pixel=256;  return fWallPositions[pixel]; }
    void SetWeightedPosition();

    Int_t Multiplicity() { return fMultiplicity; }


  private:
    //std::vector<PWFragment> PhosWallHits;
    static int fHitPatternCounter;

    Long_t   fTimeStamp;
    Short_t  fLargestHit;
    TVector3 fWeightedPosition;

    Int_t fMultiplicity;

    std::vector<TVector3> fPosition;
    std::vector<Short_t>  fACharge;
    std::vector<Short_t>  fBCharge;
    std::vector<Short_t>  fCCharge;
    std::vector<Short_t>  fPixel;
    std::vector<Short_t>  fTime;


    static TVector3 *fWallPositions[257];
    static bool      fPositionsSet;
    static void      SetWallPositions();


  ClassDef(TPhosWall,1);
};

#endif


