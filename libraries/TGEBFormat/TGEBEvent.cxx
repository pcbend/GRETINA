


#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <assert.h>

#include "TGEBEvent.h"
#include "TGEBBanksFunctions.h"

ClassImp(TGEBEvent) 

TGEBEvent::TGEBEvent() {
  fData = NULL;
  fAllocatedByUs = false;

  fEventHeader.type      = -1; 
  fEventHeader.size      =  0;
  fEventHeader.timestamp = -1;
}

void TGEBEvent::Copy(const TGEBEvent &rhs) {
  fEventHeader = rhs.fEventHeader;

  fData = (char*)malloc(fEventHeader.size);
  assert(fData);
  memcpy(fData,rhs.fData,fEventHeader.size);
  fAllocatedByUs = true;
}

TGEBEvent::TGEBEvent(const TGEBEvent &rhs) {
  Copy(rhs);
}

TGEBEvent::~TGEBEvent() { 
  Clear();
}

TGEBEvent &TGEBEvent::operator=(const TGEBEvent &rhs) {
  if(&rhs!=this)
    Clear();
  this->Copy(rhs);
  return *this;
}

void TGEBEvent::Clear(Option_t *opt) {
  if(fData && fAllocatedByUs)
    free(fData);
  fData=NULL;
  fAllocatedByUs = false;
  
  fEventHeader.type      = -1; 
  fEventHeader.size      =  0;
  fEventHeader.timestamp = -1;
}

void TGEBEvent::SetData(Int_t size, char *data) {
  fEventHeader.size = size;
  assert(!fAllocatedByUs);
  assert(IsGoodSize());
  fData = data;
  fAllocatedByUs = false;
}



Int_t TGEBEvent::GetEventType() const {
  return fEventHeader.type;
}

Int_t TGEBEvent::GetDataSize() const {
  return fEventHeader.size;
}

Long_t TGEBEvent::GetTimeStamp() const {
  return fEventHeader.timestamp;
}


char *TGEBEvent::GetData() const {
  if(!fData)
    AllocateData();
  return fData;
}

TGEBEventHeader *TGEBEvent::GetEventHeader() {
  return &fEventHeader;
}

bool TGEBEvent::IsGoodSize() const {
  return fEventHeader.size>0&&fEventHeader.size<=500*1024*1024;
}


void TGEBEvent::Print(Option_t *opt) { }


void TGEBEvent::AllocateData()  const {
  assert(!fAllocatedByUs);
  assert(IsGoodSize());
  fData = (char*)malloc(fEventHeader.size);
  assert(fData);
  fAllocatedByUs = true;
}










































