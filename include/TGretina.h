#ifndef TGRETINA_H 

#include <TObject.h>

#include <G2Fragment.h>
#include <G2Segment.h>

class TGretina : public TObject {
  
  public:
    TGretina();
    ~TGretina();


    inline const void AddGretinaHit(const G2Fragment &frag) { GretinaHits.push_back(frag); }
 
    inline const int NumberOfGretinaHits() { return GretinaHits.size(); }

    void Print(Option_t *opt = "");
    void Clear(Option_t *opt = "");

  private:
    std::vector<G2Fragment> GretinaHits;


  ClassDef(TGretina,1);
};

#endif


