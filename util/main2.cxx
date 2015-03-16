//g++ main2.cxx -I${GEBSYS}/include -L${GEBSYS}/libraries -lG2Format -lPWFormat -lGEBFormat -lGretina -lPhosWall `root-config --cflags --libs` 

#include <cstdio>

#include <TGEBFile.h>
#include <TGEBEvent.h>

#include <G2Fragment.h>
#include <G2Segment.h>

#include <PWFragment.h>
#include <PWSegment.h>

#include <TFile.h>
#include <TH1.h>
#include <TApplication.h>

#include <Globals.h>

#include <Rtypes.h>


Int_t  ReadGamma(TGEBFile &file, TGEBEvent &event) {

  int bytes = file.Read(&event); 
  while(event.GetEventType()!=1) {
    bytes = file.Read(&event); 
    if(event.GetEventType()==-1) {
      bytes = 0;
      break;
    }
//    printf("G:  type = %i \n",event.GetEventType());
  }
//  printf("found type 1\n");
  return bytes;//event.GetTimeStamp();
}


Int_t ReadParticle(TGEBFile &file, TGEBEvent &event) {
   
  int bytes = file.Read(&event); 
  while(event.GetEventType()!=17) {
    bytes = file.Read(&event); 
    if(event.GetEventType()==-1) {
      bytes = 0;
      break;
    }
//    printf("P:  type = %i \n",event.GetEventType());
  }
//  printf("found type 17\n");
  return bytes;//event.GetTimeStamp();
}



int main(int argc, char **argv) {

  TGEBFile gfile("/user/benderp/data/gamma1/1495_data68-1/Run0525/Global.dat.gz");

  TGEBFile pfile; //("/user/benderp/data/gamma1/1495_data68-1/PhoswichWall/run_525_0.cvt");
  //std::vector<TGEBFile> pwfiles;
  std::vector<std::string> pwfiles;
  pwfiles.push_back("/user/benderp/data/gamma1/1495_data68-1/PhoswichWall/run_525_0.cvt");
  pwfiles.push_back("/user/benderp/data/gamma1/1495_data68-1/PhoswichWall/run_525_1.cvt");
  pwfiles.push_back("/user/benderp/data/gamma1/1495_data68-1/PhoswichWall/run_525_2.cvt");
  int pwf_counter = 0;
  pfile.Open(pwfiles.at(pwf_counter).c_str());
 
  TGEBEvent gevent;
  TGEBEvent pevent;

  G2Fragment gfrag;
  PWFragment pfrag;

  Int_t gbytes = ReadGamma(gfile,gevent);
  Int_t pbytes = ReadParticle(pfile,pevent);

  Long_t gtime = gevent.GetTimeStamp();
  Long_t ptime = pevent.GetTimeStamp();

  TH1F *h = new TH1F("time","time",64000,0,64000);

  bool run = true;
  int x = 0;

  long gtotal = 0;
  long ptotal = 0;;

  while(run) {
    //printf("x= %i\n",x); fflush(stdout);
    if(ptime>gtime) {
      gbytes = ReadGamma(gfile,gevent);
      if(!gbytes) {
        printf("\n   EOF for %s\n",gfile.GetFileName());
        run = false;
        break;
      }
      gtotal += gbytes;
      gtime = gevent.GetTimeStamp();
    } else {
      pbytes = ReadParticle(pfile,pevent);
      if(!pbytes) { 
        pwf_counter++;
        if(pwf_counter<pwfiles.size()) {
          printf("\n   EOF for %s\n",pfile.GetFileName());
          //pfile = pwfiles.at(pwf_counter);
          pfile.Open(pwfiles.at(pwf_counter).c_str());
          printf("   Opening file %s\n",pfile.GetFileName());
          pbytes = ReadParticle(pfile,pevent);
          ptotal = 0;
        } else {
          printf("\n   EOF for %s\n",gfile.GetFileName());
          run = false;
          break;
        }
      }
      ptotal += pbytes; 
      ptime = pevent.GetTimeStamp();
    }
    h->Fill(abs(gtime-ptime));
    x++;
    if((x%150000)==0) {
       printf("   [gamma]  %.02f / %.02f  MB     |      [particle]  %.02f / %.02f  MB                \r",
              (float)gtotal/1000000.0 ,(float)gfile.GetFileSize()/1000000.0,(float)ptotal/1000000.0 ,(float)pfile.GetFileSize()/1000000.0);
       fflush(stdout);
    }

  }


  TApplication app("app",0,0);
  h->Draw();
  app.Run();



/*
  H1F *h = new TH1F("time","time",64000,0,64000);

  int max =1000000; 

  bool flag = false;

  for(int x=0;x<max+1;x++) {

  if(ptime>gtime) {
    gtime = ReadGamma(gfile,gevent);
    if(!flag) {
      //printf(DGREEN   "Reading[GAMMA]:  abs( " BLUE "%lu " DGREEN   " - " DRED " %lu )" DGREEN   " = " DYELLOW  " %lu " RESET_COLOR " \n",gtime,ptime,abs(gtime-ptime));
      h->Fill(abs(gtime-ptime));
    }
    flag = true;
  }
  else {
    ptime = ReadParticle(pfile,pevent);
    if(flag) {
      //printf(DMAGENTA "Reading[PARTI]:  abs( " BLUE "%lu " DMAGENTA " - " DRED " %lu )" DMAGENTA " = " DYELLOW  " %lu " RESET_COLOR " \n",gtime,ptime,abs(gtime-ptime));
      h->Fill(abs(gtime-ptime));
    }
    flag = false;
  }
  
    if((x%15000)==0)
      printf("on %i/%i                  \r",x,max);
  }
  printf("on %i/%i                  \n",max,max);

  TApplication app("app",0,0);
  h->Draw();
  app.Run();
  */


  return 0;

}













