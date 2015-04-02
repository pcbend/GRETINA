
#include <TGEBMultiFile.h>
#include <TGEBFile.h>
#include <TGEBEvent.h>

#include <iostream>
#include <algorithm>

#include <Globals.h>

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
  int zero = 0;
  size_t zero_t = 0;
  fEventCounter.push_back(zero);
  fLastTimeStamp.push_back(zero_t);

  fBytes.push_back(file->GetFileSize());
  fBytesRead.push_back(zero_t);

  GetNextEvent((int)fFiles.size()-1);
  return true;
}

bool TGEBMultiFile::GetNextEvent(int i) {
  fLastTimeStamp.at(i) = fEvents.at(i)->GetTimeStamp();
  size_t bytes = fFiles.at(i)->Read(fEvents.at(i));
  if(bytes!=-1)
    fBytesRead.at(i) += bytes;

  //printf("bytes = %i\n",bytes);
  if(bytes<1 && i>-1) {
    delete fFiles.at(i); 
    fFiles.erase(fFiles.begin()+i);
    delete fEvents.at(i);
    fEvents.erase(fEvents.begin()+i);

    fEventCounter.erase(fEventCounter.begin()+i);
    fLastTimeStamp.erase(fLastTimeStamp.begin()+i);
    fBytes.erase(fBytes.begin()+i);
    fBytesRead.erase(fBytesRead.begin()+i);

  }
  return true;
}


void TGEBMultiFile::InitiMultFiles()  {  }

void TGEBMultiFile::Close()  {  }

TGEBEvent *TGEBMultiFile::Read()  { 
  //int pos = std::min_element(std::begin(fEvents),std::end(fEvents)) - std::begin(fEvents); 
  int pos = FindMinimum();
  //printf("FindMinimum = %i\n",pos);
  if(pos<0)
     return 0;
  //printf("pos = %i\n",pos);
  //std::cout << "pos = " <<  std::min_element(std::begin(fEvents),std::end(fEvents)) - std::begin(fEvents) << std::endl; 
  TGEBEvent *gevent = fEvents.at(pos); 
  fEventCounter.at(pos)++;
  //printf("eventtype %i        |   timestamp  %lu  \n",gevent->GetEventType(),gevent->GetTimeStamp());
  GetNextEvent(pos);
  return gevent;
} 




void TGEBMultiFile::Clear(Option_t *opt) {  }

void TGEBMultiFile::Print(Option_t *opt) { 
 
  for(int x=0;x<fEvents.size();x++) {
    printf("[%.02f/%.02f ] [%08i]   file: %s\t" DYELLOW "Last TimeStamp read: %lu \t" DRED "Current TimeStamp: %lu\t" MAGENTA  "%lu" RESET_COLOR "\n",
            (float)fBytesRead.at(x)/1000000.0,(float)fBytes.at(x)/1000000.0,
            fEventCounter.at(x),fFiles.at(x)->GetFileName(),fLastTimeStamp.at(x),fEvents.at(x)->GetTimeStamp(),fEvents.at(x)->GetTimeStamp()-fLastTimeStamp.at(x) );
    
  }
  printf(DMAGENTA "==========================" RESET_COLOR "\n");
}


int TGEBMultiFile::FindMinimum() {
  if(fEvents.size()==0)
    return -1;
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
