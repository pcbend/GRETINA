#ifndef TGRETINA_H 
#define TGRETINA_H 

#include <TObject.h>
#include <TMath.h>

#include <TClonesArray.h>

#include <G2Fragment.h>
#include <G2Segment.h>

#include <TGretinaHit.h>

class TGretina : public TObject {
  
  public:
    TGretina();
    ~TGretina();

    void AddGretinaHit(TGretinaHit &hit);   //{ gretina_hits.push_back(hit); }
    inline const int NumberOfGretinaHits() { return gretina_hits.GetEntries(); }  //size(); }//GretinaHits.size(); }
    TGretinaHit *GetHit(const int &i)      { return (TGretinaHit*)gretina_hits.At(i); }
    inline const int NumberOfAddbackHits() { return addback_hits.GetEntries(); }  //.size(); }//GretinaHits.size(); }
    TGretinaHit *GetAddbackHit(const int &i) { return (TGretinaHit*)addback_hits.At(i); }


    void Print(Option_t *opt = "");
    void Clear(Option_t *opt = "");

    void BuildAddBack();

    static TVector3 FindPosition(int cryId,
                                 Float_t localX=0,Float_t localY=0,Float_t localZ=0);

  private:
    //std::vector<TGretinaHit> gretina_hits;
    //std::vector<TGretinaHit> addback_hits;
    //std::vector<TGretinaHit> tracked_hits;
    TClonesArray gretina_hits;//("TGretinaHit");
    TClonesArray addback_hits;//("TGretinaHit");

    static Float_t crmat[32][4][4][4];
    static void SetCRMAT();
    static bool fCRMATSet;


  ClassDef(TGretina,3);
};





#endif
  
  
  
  
  
  
  
  
