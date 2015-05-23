#ifndef TPHOSWALL_H 
#define TPHOSWALL_H 

#include <TObject.h>
#include <TVector3.h>

#include <PWFragment.h>
#include <PWSegment.h>

#include <map>

class TPhosWall : public TObject {
  
  public:
    TPhosWall();
    ~TPhosWall();

    void AddPWHit(PWFragment &frag);
    int  Size() { return fTime.size(); }

    //const inline Int_t    GetA()        { if(fLargestHit>=fTime.size()) return 0; return (Int_t)fACharge.at(fLargestHit); }
    const inline Int_t    GetA()        { if(Size()<1) return -1; return A(GetLargestNumber()); }
    //const inline Int_t    GetB()        { if(fLargestHit>=fTime.size()) return 0; return (Int_t)fBCharge.at(fLargestHit); }
    const inline Int_t    GetB()        { if(Size()<1) return -1; return B(GetLargestNumber()); }
    //const inline Int_t    GetC()        { if(fLargestHit>=fTime.size()) return 0; return (Int_t)fCCharge.at(fLargestHit); }
    const inline Int_t    GetC()        { if(Size()<1) return -1; return C(GetLargestNumber()); }
                 Float_t  GetACal();    
                 Float_t  GetBCal();    
                 Float_t  GetCCal();    

    const inline Int_t    GetBDiff()    {if(Size()<1) return -1; if(GetLargestNumber()>Size()) return -1; return BDiffCal(GetLargestNumber());}


    //const inline Int_t    GetTime()     { if(fLargestHit>=fTime.size()) return 0; return (Int_t)fTime.at(fLargestHit);    }
    const inline Int_t    GetTime()     { if(Size()<1) return -1; if(GetLargestNumber()>Size()) return -1; return Time(GetLargestNumber()); }
          const  Int_t    GetLargestNumber() { return fLargestHit; }  
    //const inline Int_t    GetPixel()    { if(fLargestHit>=fPixel.size()) return -1; return (Int_t)fPixel.at(fLargestHit); }
    const inline Int_t    GetPixel()    { if(Size()<1) return -1; if(GetLargestNumber()>Size()) return -1; return Pixel(GetLargestNumber()); }
    const inline TVector3 GetPosition() { return fWeightedPosition; }

    const inline Int_t    GetASum()   { int chg=0; for(int x=0;x<fACharge.size();x++) {chg+=fACharge.at(x);} return chg;}
    const inline Int_t    GetBSum()   { int chg=0; for(int x=0;x<fBCharge.size();x++) {chg+=fBCharge.at(x);} return chg;}
    const inline Int_t    GetCSum()   { int chg=0; for(int x=0;x<fCCharge.size();x++) {chg+=fCCharge.at(x);} return chg;}

    const inline Int_t    GetBDiffSum() { int chg=0; for(int x=0;x<Size();x++) {chg+=BDiffCal(x);} return chg;}

    const inline Int_t    A(const int &i)        { if((i<1)||(i>(Size()-1))) return -1; return (Int_t)fACharge.at(i); }
    const inline Int_t    B(const int &i)        { if((i<1)||(i>(Size()-1))) return -1; return (Int_t)fBCharge.at(i); }
    const inline Int_t    C(const int &i)        { if((i<1)||(i>(Size()-1))) return -1; return (Int_t)fCCharge.at(i); }
               Float_t    ACal(const int &i);
               Float_t    BCal(const int &i);
               Float_t    CCal(const int &i);
                 Int_t    BDiffCal(const int &i);
    const inline Int_t    Time(const int &i)     { if((i<1)||(i>(Size()-1))) return -1; return (Int_t)fTime.at(i);    }
    const inline Int_t    Pixel(const int &i)    { if((i<1)||(i>(Size()-1))) return -1; return (Int_t)fPixel.at(i); }

    Int_t    GetASmartSum(float res = 20.0,int threshold=200); 
    Int_t    GetBSmartSum(float res = 20.0,int threshold=200);
    Int_t    GetCSmartSum(float res = 20.0,int threshold=200); 
    //inline const int NumberOfPWHits() { return PhosWallHits.size(); }

    Float_t  GetACalSmartSum(float res = 20.0,Float_t threshold=0.10); 
    Float_t  GetBCalSmartSum(float res = 20.0,Float_t threshold=0.10);
    Float_t  GetCCalSmartSum(float res = 20.0,Float_t threshold=0.10); 



    void Print(Option_t *opt = "");
    void Clear(Option_t *opt = "");
    void Draw(Option_t *opt="");
    void DrawXY(Option_t *opt="");

    static TVector3 *FindWallPosition(Int_t &pixel) { if(pixel<0||pixel>256) pixel=256;  return fWallPositions[pixel]; }
    void FindWeightedPosition();
    void SetWeightedPosition(const TVector3 &temp)  { fWeightedPosition = temp; }    

    TVector3 GetWallPosition(int pixelnumber,double delta=5.0);

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


    void SetCalMaps();
    static bool fCalMapsSet;
    static std::map<int,std::pair<float,float> > fAMap;
    static std::map<int,std::pair<float,float> > fBMap;
    static std::map<int,std::pair<float,float> > fCMap;

    static std::map<int,int> fBDiffMap;

  ClassDef(TPhosWall,1);
};

#endif


