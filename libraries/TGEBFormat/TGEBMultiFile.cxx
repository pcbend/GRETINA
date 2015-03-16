
#include <TGEBMultiFile.h>
#include <TGEBFile.h>
#include <TGEBEvent.h>

#include <iostream>
#include <algorithm>

ClassImp(TGEBMultiFile)


TGEBMultiFile::TGEBMultiFile() { }

TGEBMultiFile::~TGEBMultiFile() { }

bool TGEBMultiFile::Add(const char *fname) {
  TGEBFile *file = new TGEBFile(fname);
  Add(file);
  return true;
}

bool TGEBMultiFile::Add(TGEBFile *file) {
  fFiles.push_back(file);
  TGEBEvent *event = new TGEBEvent;
  fEvents.push_back(event);
  int counter = 0;
  fEventCounter.push_back(counter);
  GetNextEvent((int)fFiles.size()-1);
  return true;
}

bool TGEBMultiFile::GetNextEvent(int i) {
  int bytes = fFiles.at(i)->Read(fEvents.at(i));
  printf("bytes = %i\n",bytes);
  if(bytes<1) {
    fFiles.erase(fFiles.begin()+i);
    fEvents.erase(fEvents.begin()+i);
  }
  return true;
}


void TGEBMultiFile::InitiMultFiles()  {  }

void TGEBMultiFile::Close()  {  }

TGEBEvent *TGEBMultiFile::Read()  { 
  //int pos = std::min_element(std::begin(fEvents),std::end(fEvents)) - std::begin(fEvents); 
  int pos = FindMinimum();


  printf("pos = %i\n",pos);
  std::cout << "pos = " <<  std::min_element(std::begin(fEvents),std::end(fEvents)) - std::begin(fEvents) << std::endl; 
  TGEBEvent *gevent = fEvents.at(pos); 
  fEventCounter.at(pos)++;
  printf("eventtype %i        |   timestamp  %lu  \n",gevent->GetEventType(),gevent->GetTimeStamp());
  GetNextEvent(pos);
  return gevent;
} 




void TGEBMultiFile::Clear(Option_t *opt) {  }

void TGEBMultiFile::Print(Option_t *opt) {  }


int TGEBMultiFile::FindMinimum() {
  if(fEvents.size()==1)
    return 0;
  int smallest = 0;
  Long_t last = fEvents.at(0)->GetTimeStamp();
  int x;
  for(x=1;x<fEvents.size();x++) {
    if(fEvents.at(x)->GetTimeStamp()<last) {
      smallest = x;
      last = fEvents.at(x)->GetTimeStamp();
    }
  }
  return smallest;
}
