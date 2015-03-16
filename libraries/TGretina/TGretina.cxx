

#include <TGretina.h>


ClassImp(TGretina)


TGretina::TGretina() { Clear(); }

TGretina::~TGretina() {  }

void TGretina::Print(Option_t *opt) { }

void TGretina::Clear(Option_t *opt) {
  GretinaHits.clear();
}


