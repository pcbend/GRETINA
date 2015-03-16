#ifndef TGRETINA_H 

#include <TObject.h>

#include <PWFragment.h>
#include <PWSegment.h>

class TPhosWall : public TObject {
  
  public:
    TPhosWall();
    ~TPhosWall();


    inline const void AddPWHit(const PWFragment &frag) { PhosWallHits.push_back(frag); }
 
    inline const int NumberOfPWHits() { return PhosWallHits.size(); }

    void Print(Option_t *opt = "");
    void Clear(Option_t *opt = "");

  private:
    std::vector<PWFragment> PhosWallHits;


  ClassDef(TPhosWall,1);
};

#endif


