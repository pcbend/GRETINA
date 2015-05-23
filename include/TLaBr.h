#ifndef TLABR_H 
#define TLABR_H 

#include <TObject.h>
#include <TMath.h>

#include <G2Fragment.h>
#include <G2Segment.h>

#include <TLaBrHit.h>

class TLaBr : public TObject {
  
  public:
    TLaBr();
    TLaBr(LaBrFragment&);
    ~TLaBr();

    void Set(LaBrFragment&);

    void AddLaBrHit(TLaBrHit &hit)         { labr_hits.push_back(hit); }
    inline const int NumberOfHits()        { return labr_hits.size(); }//LaBrHits.size(); }
    TLaBrHit *GetHit(const int &i)         { return &labr_hits.at(i); }

    void Print(Option_t *opt = "") const;
    void Clear(Option_t *opt = "");

    static TVector3 FindPosition(int cryId);


  private:
    std::vector<TLaBrHit> labr_hits;

  ClassDef(TLaBr,1);
};





#endif
  
  
  
  
  
  
  
  
