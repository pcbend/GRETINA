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

    const inline Short_t    GetA()        { if(fLargestHit>=fTime.size()) return 0; return fACharge.at(fLargestHit); }
    const inline Short_t    GetB()        { if(fLargestHit>=fTime.size()) return 0; return fBCharge.at(fLargestHit); }
    const inline Short_t    GetC()        { if(fLargestHit>=fTime.size()) return 0; return fCCharge.at(fLargestHit); }
    const inline Short_t    GetTime()     { if(fLargestHit>=fTime.size()) return 0; return fTime.at(fLargestHit);    }
    const inline Short_t    GetLargestNumber() { return fLargestHit; }  

    //inline const int NumberOfPWHits() { return PhosWallHits.size(); }

    void Print(Option_t *opt = "");
    void Clear(Option_t *opt = "");

    static TVector3 *FindWallPosition(const int &pixel) { return fWallPositions[pixel]; }

  private:
    //std::vector<PWFragment> PhosWallHits;


    Long_t  fTimeStamp;
    Short_t fLargestHit;

    std::vector<TVector3> fPosition;
    std::vector<Short_t>  fACharge;
    std::vector<Short_t>  fBCharge;
    std::vector<Short_t>  fCCharge;
    std::vector<Short_t>  fPixel;
    std::vector<Short_t>  fTime;


    static TVector3 *fWallPositions[256];
    static bool      fPositionsSet;
    static void      SetWallPositions();


  ClassDef(TPhosWall,1);
};

#endif


