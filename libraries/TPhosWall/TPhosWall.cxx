

#include <TPhosWall.h>


ClassImp(TPhosWall)


TPhosWall::TPhosWall() { Clear(); }

TPhosWall::~TPhosWall() {  }

void TPhosWall::Print(Option_t *opt) { }

void TPhosWall::Clear(Option_t *opt) {
  PhosWallHits.clear();
}


