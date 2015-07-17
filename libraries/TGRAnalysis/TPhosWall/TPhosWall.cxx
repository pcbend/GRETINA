

#include <Globals.h>

#include <TROOT.h>

#include <TPhosWall.h>
#include <TMath.h>
#include <TRandom.h>

#include <string>
#include <sstream>
#include <fstream>
#include <utility>

ClassImp(TPhosWall)


TVector3 *TPhosWall::beam = new TVector3(0,0,1);
int TPhosWall::fHitPatternCounter=0;


bool TPhosWall::fCalMapsSet = false;

std::map<int,std::pair<float,float> > TPhosWall::fAMap;
std::map<int,std::pair<float,float> > TPhosWall::fBMap;
std::map<int,std::pair<float,float> > TPhosWall::fCMap;


std::map<int,int> TPhosWall::fBDiffMap;



TPhosWall::TPhosWall() { 
  fPosition = 0;
  fAddress = 0; 
  fACharge = 0; 
  fBCharge = 0; 
  fCCharge = 0; 
  fPixel = 0;   
  fTime = 0;    

  if(!fPositionsSet)
    SetWallPositions();
  if(!fCalMapsSet)
    SetCalMaps();

}

TPhosWall::~TPhosWall() { Clear();  }


TPhosWall::TPhosWall(TGEBEvent &event) {
  fPosition = 0;
  fAddress = 0; 
  fACharge = 0; 
  fBCharge = 0; 
  fCCharge = 0; 
  fPixel = 0;   
  fTime = 0;    

  if(!fPositionsSet)
     SetWallPositions();
  if(!fCalMapsSet)
     SetCalMaps();
  Copy(event);
}



void TPhosWall::Copy(const TGEBEvent &event) {
  Clear(); 
   if(!fPositionsSet)
      SetWallPositions();
   if(!fCalMapsSet)
      SetCalMaps();

  fTimeStamp = event.GetTimeStamp();
  Int_t fLargestValue = 0;
  fLargestHit         = 0; 
  fNumberOfHits       = 0;
  int x               = 0;
  
  //fNumberOfHits = event.GetDataSize()/sizeof(TGEBEvent::TPWHit); 
  int hitsize = event.GetDataSize()/sizeof(TGEBEvent::TPWHit); 
  fPosition = new TVector3[hitsize]; 
  fAddress  = new Int_t[hitsize];  
  fACharge  = new Short_t[hitsize];  
  fBCharge  = new Short_t[hitsize];  
  fCCharge  = new Short_t[hitsize];  
  fPixel    = new Short_t[hitsize];      
  fTime     = new Short_t[hitsize];       

  //printf("Memory set.\n");

  for(int x=0;x<event.GetDataSize();x+=sizeof(TGEBEvent::TPWHit)) {
    TGEBEvent::TPWHit *hit = ((TGEBEvent::TPWHit*)(event.GetData()+x));
    if(fNumberOfHits==hitsize)
       break;
    fACharge[fNumberOfHits]  = hit->data_a; 
    fBCharge[fNumberOfHits]  = hit->data_b; 
    fCCharge[fNumberOfHits]  = hit->data_c; 
    fPixel[fNumberOfHits]    = hit->pix_id;
    fTime[fNumberOfHits]     = hit->time;
    fAddress[fNumberOfHits]  = ((int)(event.GetEventType())<<24)+(int)(Pixel(fNumberOfHits)); 
    fPosition[fNumberOfHits] = *(FindWallPosition(Pixel(Size())));//fNumberOfHits)));

    if(B(fNumberOfHits)>fLargestValue) {
      fLargestValue = B(fNumberOfHits);
      fLargestHit=fNumberOfHits;
    }
    fNumberOfHits++;
  }

}

/*
void TPhosWall::AddPWHit(PWFragment &frag) {
   Clear();
   fTimeStamp  = frag.GetTimeStamp();
   Int_t fLargestValue = 0;
   fLargestHit = 0; //frag.GetLargestNumber();

   int x = 0;
   for(x=0;x<frag.GetNumberOfHits();x++) {
      fAddress.push_back(frag.Address(x));
      fACharge.push_back(frag.GetSegment((unsigned int&)x).A());
      fBCharge.push_back(frag.GetSegment((unsigned int&)x).B());
      fCCharge.push_back(frag.GetSegment((unsigned int&)x).C());
      fPixel.push_back(frag.GetSegment((unsigned int&)x).Pixel());
      fTime.push_back(frag.GetSegment((unsigned int&)x).Time());
      if(fPixel.back()>-1&&fPixel.back()<256) {
         //printf("here1,  %i  \n",fPixel.back());
         //printf("here2,  0x%08x  \n",fWallPositions[fPixel.back()]);
         fPosition.push_back(*fWallPositions[fPixel.back()]);
      }
      else
         fPosition.push_back(frag.GetSegment((unsigned int&)x).Position());

      //if(B(x)>fLargestValue) {
      if(fBCharge.back()>fLargestValue) {
         fLargestValue = fBCharge.back();
         fLargestHit=x;
      }
   }
   //printf(" Added %i PhosWall hits.\n",x);
}
*/
/*
const Int_t TPhosWall::GetLargestNumber() {
  Int_t temp_largestvalue=0;
  Int_t largest_x = 0;
  for(int x=0;x<Size();x++) {
    if(BDiffCal(x)>temp_largestvalue)
       largest_x = x;
       //fLargestHit=x;
  }
  //printf("largest number = %i\n");
  return largest_x; //fLargestHit;
}
*/


void TPhosWall::FindWeightedPosition() {


  return;
/*
  if(fNumberOfHits<=fLargestHit)
     return;
  float ChargeSum=0.0;
  float MaxPixelDistance = 20.0;
  for(int i=0;i<fNumberOfHits;i++) {
    if((*Position(fLargestHit)-*Position(i)).Mag()<MaxPixelDistance) 
      ChargeSum += BCal(i);
  }
  TVector3 result =  *Position(fLargestHit);
  result *= ((BCal(fLargestHit))/ChargeSum);
  double x =0.0;
  double y =0.0;
  double z =0.0;
 

  //printf(DRED "START:" RESET_COLOR "\n");

  for(int i=0;i<fNumberOfHits;i++) {
    //printf("Mag: %.02f\tBCal = %.02f\n",(fPosition.at(fLargestHit)-fPosition.at(i)).Mag(),BCal(i));
    if(i==fLargestHit) {
       //printf(DGREEN);
       //printf("[%02i][%03i]  ",i,fPixel.at(i)); fPosition.at(i).Print();
       //printf(RESET_COLOR);
       continue;
    }
    //printf("[%02i][%03i]  ",i,fPixel.at(i)); fPosition.at(i).Print();
    if( ((*Position(fLargestHit)-*Position(i)).Mag()<MaxPixelDistance) &&
        ((Pixel(fLargestHit)/64) == (Pixel(i)/64)) ) {
      TVector3 temp = ((BCal(i))/ChargeSum)*(*Position(i));
      result += temp;
    } else {
    
    }
  }
  SetWeightedPosition(result);
  //printf(DYELLOW);
  //printf("\t[%02i]",fMultiplicity);fWeightedPosition.Print();
  //printf(RESET_COLOR);
  //printf("----------------------------------\n");
*/
}

void TPhosWall::Print(Option_t *opt) {
  
  for(int i=0;i<fNumberOfHits;i++) {
    if(i==fLargestHit) {
       printf(DGREEN);
       printf("[%02i][%03i]  \t",i,Pixel(i)); //fPosition.at(i).Print();
       printf("Mag: %.02f\tAChg = %i\t|\tBChg = %i\n",(*Position(fLargestHit)-*Position(i)).Mag(),
       A(i),B(i));
       printf(RESET_COLOR);
       continue;
    }
    printf("[%02i][%03i]  \t",i,Pixel(i)); //fPosition.at(i).Print();
    printf("Mag: %.02f\tAChg = %i\t|\tBChg = %i\n",(*Position(fLargestHit)-*Position(i)).Mag(),
    A(i),B(i));
  }
  printf(DYELLOW);
  fWeightedPosition.Print();
  printf(RESET_COLOR);
  printf("----------------------------------\n");
}

void TPhosWall::Clear(Option_t *opt) {

   //printf("Clear called.\n");

   fTimeStamp  = 0;
   fLargestHit = 0;
   fWeightedPosition.SetXYZ(0,0,1);

   fNumberOfHits = 0;
/*
   if(fACharge)  delete fPosition;  fACharge = 0;
   if(fBCharge)  delete fPosition;  fBCharge = 0;
   if(fCCharge)  delete fPosition;  fCCharge = 0;
   if(fPixel)    delete fPosition;  fPixel = 0;  
   if(fTime)     delete fPosition;  fTime = 0;   
   if(fAddress)  delete fPosition;  fAddress = 0;
   if(fPosition) delete fPosition;  fPosition = 0;
*/
}



Int_t   TPhosWall::GetASmartSum(float res,int threshold) { 
  Int_t chg = 0;
  if(Size()<1)
     return chg;
  int lpixel = GetPixel();
  TVector3 lvec = *(FindWallPosition(lpixel));
  for(int y=0;y<Size();y++) {
    int cpixel = Pixel(y);
    if((cpixel/64)!=(lpixel/64))
       continue;
    TVector3 cvec = *(FindWallPosition(cpixel));
    float mag = (lvec-cvec).Mag();
    if(mag<res && (A(y)>threshold))
      chg += A(y);
  }                                                          
  return chg;
}


Int_t   TPhosWall::GetBSmartSum(float res,int threshold) {
  Int_t chg = 0;
  if(Size()<1)
     return chg;
  int lpixel = GetPixel();
  TVector3 lvec = *(FindWallPosition(lpixel));
  for(int y=0;y<Size();y++) {
    int cpixel = Pixel(y);
    if((cpixel/64)!=(lpixel/64))
       continue;
    TVector3 cvec = *(FindWallPosition(cpixel));
    float mag = (lvec-cvec).Mag();
    if(mag<res && (B(y)>threshold))
      chg += B(y);
  }                                                         
  return chg;
}


Int_t   TPhosWall::GetCSmartSum(float res,int threshold) { 
  Int_t chg = 0;
  if(Size()<1)
     return chg;
  int lpixel = GetPixel();
  TVector3 lvec = *(FindWallPosition(lpixel));
  for(int y=0;y<Size();y++) {
    int cpixel = Pixel(y);
    if((cpixel/64)!=(lpixel/64))
       continue;
    TVector3 cvec = *(FindWallPosition(cpixel));
    float mag = (lvec-cvec).Mag();
    if(mag<res && (C(y)>threshold))
      chg += C(y);
  }                                                         
  return chg;
}


void TPhosWall::Draw(Option_t *opt) {
  TH2I hitpat[4];

  std::string exef;
  std::string exex = ".x ";
  if(strlen(opt)!=0) {
    exef.append(opt);
    exex.append(opt);
    exex.append(".C");
  }
  for(int x=0;x<4;x++) {
     hitpat[x] = TH2I(Form("hit%i",x),Form("Pixels %03i - %03i",x*64,x*64+63),8,0,8,8,0,8);
     hitpat[x].GetXaxis()->SetNdivisions(8);
     hitpat[x].GetYaxis()->SetNdivisions(8);
     hitpat[x].SetStats(false);
  }
  for(int x=0;x<fNumberOfHits;x++) {
    int hist = Pixel(x)/64;
    int row  = ((Pixel(x)-(hist*64))%8);
    //if(strcmp(opt,"order")==0) {
    //  printf("reordering happening.\n");
    //  if(col==7) col=5;
    //  else if(col==6) col=7;
    //  else if(col==5) col=6;
    //}
    int col  = (Pixel(x)-(hist*64))/8;
    //if(strcmp(opt,"order")) {
    //   row = abs(row-7);
    //}
    hitpat[hist].Fill(row,col,A(x));
    printf("\t\tPixel[%03i | %02i]  r[%02i] c[%02i]   %i\n",Pixel(x),Pixel(x)-hist*64,row,col,A(x));
  }
  TCanvas *c;
  if(gPad) {
    if(!gPad->IsEditable())
      gROOT->MakeDefCanvas();
    //else   
    //  gPad->GetCanvas()->Clear();
  } else {
      gROOT->MakeDefCanvas();
  }
  //c = gPad->GetCanvas();
  TVirtualPad *mother = gPad;
  mother->Divide(2,2);
  mother->cd(1); hitpat[0].DrawCopy("colz");
  gPad->SetGrid();
  if(exef.size()) gPad->AddExec(exef.c_str(),exex.c_str());
  mother->cd(2); hitpat[1].DrawCopy("colz");
  gPad->SetGrid();
  if(exef.size()) gPad->AddExec(exef.c_str(),exex.c_str());
  mother->cd(4); hitpat[2].DrawCopy("colz");
  gPad->SetGrid();
  if(exef.size()) gPad->AddExec(exef.c_str(),exex.c_str());
  mother->cd(3); hitpat[3].DrawCopy("colz");
  gPad->SetGrid();
  if(exef.size()) gPad->AddExec(exef.c_str(),exex.c_str());
  gPad->Modified();
  gPad->Update();
  return;
}


void TPhosWall::DrawXY(Option_t *opt) {
  TH2I hitpattern("hitpattern","PWFragmet XY Hit Pattern",
                   116,-58,58,116,-58,58);
  for(int i=0;i<fNumberOfHits;i++) {
     TVector3 vec = GetWallPosition(Pixel(i));
     hitpattern.Fill(vec.X(),
                     vec.Y(),
                     A(i));
  }
  //TCanvas *c;
  if(gPad) {
    if(!gPad->IsEditable())
      gROOT->MakeDefCanvas();
    //else   
    //  gPad->GetCanvas()->Clear();
  } else {
      gROOT->MakeDefCanvas();
  }
  //c = gPad->GetCanvas();
  hitpattern.DrawCopy("colz");
  gPad->SetGrid();
  gPad->Modified();
  gPad->Update();
  return;
}















// average distance = 5.5 cm
//
// phi1 = phi
// phi2 = phi + 270
// phi3 = phi + 180
// phi4 = phi +  90


TVector3 *TPhosWall::fWallPositions[257];

bool TPhosWall::fPositionsSet = false;

TVector3 TPhosWall::GetWallPosition(int pixelnumber,double delta) {
   //printf("Calculating PhosWall positions.\n");

   int det   = pixelnumber/64;
   int pixel = pixelnumber - (64*det);
   TVector3 position; 

   double L   = 49.0;
   double Ro  = 55.0;
   double Psi = 50.6*TMath::DegToRad();

      

   //int x = pixel/8;           
//   int x = (pixel%8);
   //int y = abs((pixel%8)-7);  
//   int y = (pixel/8);

   int x,y;  //1 to 8!!
   switch(det) {
      case 0:
         x = (pixel%8);
         y = (pixel/8);
         //delta = (double)y*2.5;
         break;
      case 1:
         x = (pixel/8) + 1;
         y = abs((pixel%8)-7);
         //delta = (double)x*2.5;
         break;
      case 2:
         x = abs((pixel%8)-7);
         y = abs((pixel/8)-7);
         //delta = (double)y*2.5;
         break;
      case 3:
         x = abs((pixel/8)-7);
         y = (pixel&8) + 1; 
         //delta = (double)x*2.5;
         break;
   }
   x++; y++;
   double localx = ((double(x)-4.5)*6.08);  
   double localy = ((double(y)-4.5)*6.08);  
   //------------------------------------------------//
   //------------------------------------------------//
   //------------------------------------------------//
   //------------------------------------------------//
      // step sizes are ackward.  from 1-8 in either x or y:
      //
      // 1:    3.13
      // 2:    9.30
      // 3:   15.38
      // 4:   21.46
      // 5:   27.54
      // 6:   32.62
      // 7:   39.70
      // 8:   45.87
   //------------------------------------------------//
   //------------------------------------------------//
   //------------------------------------------------//
   //------------------------------------------------//







   double globalx = (L/2.0) - localx + delta;
   double globaly = Ro*TMath::Sin(Psi) - ((L/2.0)-localy)*TMath::Cos(Psi) - delta;
   double globalz = Ro*TMath::Cos(Psi) + ((L/2.0)-localy)*TMath::Sin(Psi);
/*
   switch(det) {
      case 0:
        globalx =  globalx;
        globaly =  globaly;
        break;
      case 1:
        globalx = -globalx;
        globaly =  globaly;
        break;
      case 2:
        globalx = -globalx;
        globaly = -globaly;
        break;
      case 3:
        globalx =  globalx;
        globaly = -globaly;
        break;
   };
*/
   position.SetXYZ(globalx,globaly,globalz);
   position.RotateZ(((double)(det))*TMath::PiOver2());
   //position.SetPhi(position.Phi()+(double)det*TMath::PiOver2());

   return position;
}


void TPhosWall::SetWallPositions() {
   printf("Setting PhosWall positions.\n");
   int detoffset;
   double phi;
   for(int i=0;i<4;i++) {
      if(i==0)      {detoffset = 0;    phi = TMath::DegToRad()*0   + TMath::PiOver2() ;}
      else if(i==1) {detoffset = 64;   phi = TMath::DegToRad()*270 + TMath::PiOver2() ;}
      else if(i==2) {detoffset = 64*2; phi = TMath::DegToRad()*180 + TMath::PiOver2() ;}
      else if(i==3) {detoffset = 64*3; phi = TMath::DegToRad()*90  + TMath::PiOver2() ;}

      fWallPositions[0  + detoffset] = new TVector3;  fWallPositions[0 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*37.141,TMath::DegToRad()*20.966 +  phi);
      fWallPositions[1  + detoffset] = new TVector3;  fWallPositions[1 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*32.895,TMath::DegToRad()*24.772 +  phi);
      fWallPositions[2  + detoffset] = new TVector3;  fWallPositions[2 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*28.445,TMath::DegToRad()*30.019 +  phi);
      fWallPositions[3  + detoffset] = new TVector3;  fWallPositions[3 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*23.908,TMath::DegToRad()*37.687 +  phi);
      fWallPositions[4  + detoffset] = new TVector3;  fWallPositions[4 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*19.653,TMath::DegToRad()*49.364 +  phi);
      fWallPositions[5  + detoffset] = new TVector3;  fWallPositions[5 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*16.391,TMath::DegToRad()*67.124 +  phi);   
      fWallPositions[6  + detoffset] = new TVector3;  fWallPositions[6 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*15.165,TMath::DegToRad()*89.178 + phi);  
      fWallPositions[7  + detoffset] = new TVector3;  fWallPositions[7 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*16.580,TMath::DegToRad()*65.538 + phi);  

      fWallPositions[8  + detoffset] = new TVector3;  fWallPositions[8 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*40.971,TMath::DegToRad()*26.144 +  phi);   
      fWallPositions[9  + detoffset] = new TVector3;  fWallPositions[9 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*36.941,TMath::DegToRad()*30.589 +  phi);   
      fWallPositions[10 + detoffset] = new TVector3;  fWallPositions[10 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*32.748,TMath::DegToRad()*36.507 +  phi);
      fWallPositions[11 + detoffset] = new TVector3;  fWallPositions[11 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*28.546,TMath::DegToRad()*44.701 +  phi);
      fWallPositions[12 + detoffset] = new TVector3;  fWallPositions[12 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*24.730,TMath::DegToRad()*56.18  +  phi);
      fWallPositions[13 + detoffset] = new TVector3;  fWallPositions[13 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*21.943,TMath::DegToRad()*71.77 +   phi);
      fWallPositions[14 + detoffset] = new TVector3;  fWallPositions[14 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*20.940,TMath::DegToRad()*89.358 + phi);
      fWallPositions[15 + detoffset] = new TVector3;  fWallPositions[15 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*22.100,TMath::DegToRad()*70.448 + phi);

      fWallPositions[16 + detoffset] = new TVector3;  fWallPositions[16 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*45.272,TMath::DegToRad()*30.836  + phi);
      fWallPositions[17 + detoffset] = new TVector3;  fWallPositions[17 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*41.556,TMath::DegToRad()*35.713  + phi);
      fWallPositions[18 + detoffset] = new TVector3;  fWallPositions[18 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*37.721,TMath::DegToRad()*41.992  + phi);
      fWallPositions[19 + detoffset] = new TVector3;  fWallPositions[19 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*33.932,TMath::DegToRad()*50.277  + phi);
      fWallPositions[20 + detoffset] = new TVector3;  fWallPositions[20 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*30.574,TMath::DegToRad()*61.151  + phi);
      fWallPositions[21 + detoffset] = new TVector3;  fWallPositions[21 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*28.196,TMath::DegToRad()*74.846  + phi);
      fWallPositions[22 + detoffset] = new TVector3;  fWallPositions[22 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*27.361,TMath::DegToRad()*89.472 + phi);
      fWallPositions[23 + detoffset] = new TVector3;  fWallPositions[23 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*28.328,TMath::DegToRad()*73.722 + phi);

      fWallPositions[24 + detoffset] = new TVector3;  fWallPositions[24 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*50.035,TMath::DegToRad()*35.110  + phi);
      fWallPositions[25 + detoffset] = new TVector3;  fWallPositions[25 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*46.724,TMath::DegToRad()*40.255  + phi);
      fWallPositions[26 + detoffset] = new TVector3;  fWallPositions[26 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*43.330,TMath::DegToRad()*46.672  + phi);
      fWallPositions[27 + detoffset] = new TVector3;  fWallPositions[27 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*40.027,TMath::DegToRad()*54.797  + phi);
      fWallPositions[28 + detoffset] = new TVector3;  fWallPositions[28 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*37.149,TMath::DegToRad()*64.933  + phi);
      fWallPositions[29 + detoffset] = new TVector3;  fWallPositions[29 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*35.153,TMath::DegToRad()*77.050  + phi);
      fWallPositions[30 + detoffset] = new TVector3;  fWallPositions[30 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*34.462,TMath::DegToRad()*89.552 + phi);
      fWallPositions[31 + detoffset] = new TVector3;  fWallPositions[31 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*35.263,TMath::DegToRad()*76.075 + phi);

      fWallPositions[32 + detoffset] = new TVector3;  fWallPositions[32 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*55.164,TMath::DegToRad()*38.979  + phi);
      fWallPositions[33 + detoffset] = new TVector3;  fWallPositions[33 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*52.326,TMath::DegToRad()*44.260  + phi);
      fWallPositions[34 + detoffset] = new TVector3;  fWallPositions[34 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*49.446,TMath::DegToRad()*50.663  + phi);
      fWallPositions[35 + detoffset] = new TVector3;  fWallPositions[35 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*46.672,TMath::DegToRad()*58.492  + phi);
      fWallPositions[36 + detoffset] = new TVector3;  fWallPositions[36 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*44.293,TMath::DegToRad()*67.884  + phi);
      fWallPositions[37 + detoffset] = new TVector3;  fWallPositions[37 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*42.667,TMath::DegToRad()*78.701  + phi);
      fWallPositions[38 + detoffset] = new TVector3;  fWallPositions[38 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*42.109,TMath::DegToRad()*89.611 + phi);
      fWallPositions[39 + detoffset] = new TVector3;  fWallPositions[39 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*42.756,TMath::DegToRad()*77.843 + phi);

      fWallPositions[40 + detoffset] = new TVector3;  fWallPositions[40 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*60.528,TMath::DegToRad()*42.468  + phi);
      fWallPositions[41 + detoffset] = new TVector3;  fWallPositions[41 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*58.205,TMath::DegToRad()*47.786  + phi);
      fWallPositions[42 + detoffset] = new TVector3;  fWallPositions[42 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*55.871,TMath::DegToRad()*54.074  + phi);
      fWallPositions[43 + detoffset] = new TVector3;  fWallPositions[43 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*53.651,TMath::DegToRad()*61.546  + phi);
      fWallPositions[44 + detoffset] = new TVector3;  fWallPositions[44 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*51.772,TMath::DegToRad()*70.238  + phi);
      fWallPositions[45 + detoffset] = new TVector3;  fWallPositions[45 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*50.503,TMath::DegToRad()*79.983  + phi);
      fWallPositions[46 + detoffset] = new TVector3;  fWallPositions[46 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*50.071,TMath::DegToRad()*89.656 + phi);
      fWallPositions[47 + detoffset] = new TVector3;  fWallPositions[47 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*50.572,TMath::DegToRad()*79.217 + phi);

      fWallPositions[48 + detoffset] = new TVector3;  fWallPositions[48 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*65.983,TMath::DegToRad()*45.606  + phi);
      fWallPositions[49 + detoffset] = new TVector3;  fWallPositions[49 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*64.180,TMath::DegToRad()*50.890  + phi);
      fWallPositions[50 + detoffset] = new TVector3;  fWallPositions[50 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*62.390,TMath::DegToRad()*57.005  + phi);
      fWallPositions[51 + detoffset] = new TVector3;  fWallPositions[51 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*60.711,TMath::DegToRad()*64.098  + phi);
      fWallPositions[52 + detoffset] = new TVector3;  fWallPositions[52 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*59.308,TMath::DegToRad()*72.154  + phi);
      fWallPositions[53 + detoffset] = new TVector3;  fWallPositions[53 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*58.371,TMath::DegToRad()*81.006  + phi);
      fWallPositions[54 + detoffset] = new TVector3;  fWallPositions[54 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*58.054,TMath::DegToRad()*89.692 + phi);
      fWallPositions[55 + detoffset] = new TVector3;  fWallPositions[55 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*58.422,TMath::DegToRad()*80.315 + phi);

      fWallPositions[56 + detoffset] = new TVector3;  fWallPositions[56 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*71.343,TMath::DegToRad()*48.456  + phi);
      fWallPositions[57 + detoffset] = new TVector3;  fWallPositions[57 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*70.127,TMath::DegToRad()*53.656  + phi);
      fWallPositions[58 + detoffset] = new TVector3;  fWallPositions[58 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*68.848,TMath::DegToRad()*59.560  + phi);
      fWallPositions[59 + detoffset] = new TVector3;  fWallPositions[59 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*67.665,TMath::DegToRad()*66.274  + phi);
      fWallPositions[60 + detoffset] = new TVector3;  fWallPositions[60 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*66.691,TMath::DegToRad()*73.755  + phi);
      fWallPositions[61 + detoffset] = new TVector3;  fWallPositions[61 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*66.047,TMath::DegToRad()*81.484  + phi);
      fWallPositions[62 + detoffset] = new TVector3;  fWallPositions[62 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*65.831,TMath::DegToRad()*89.721 + phi);
      fWallPositions[63 + detoffset] = new TVector3;  fWallPositions[63 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*66.082,TMath::DegToRad()*81.219 + phi);
   }
   fWallPositions[256] = new TVector3; fWallPositions[256]->SetXYZ(0,0,1);

   fPositionsSet=true;
}



void TPhosWall::SetCalMaps() {

  fCalMapsSet = true;

  std::string fMapAfile = getenv("GEBSYS");
  std::string fMapBfile = getenv("GEBSYS");
  std::string fMapCfile = getenv("GEBSYS");
  fMapAfile.append("/libraries/TGRAnalysis/TPhosWall/Acoeff-322-8x8.xy");
  fMapBfile.append("/libraries/TGRAnalysis/TPhosWall/Bcoeff-322-8x8.xy");
  fMapCfile.append("/libraries/TGRAnalysis/TPhosWall/Ccoeff-322-8x8.xy");
  //printf("fMapAfile = %s\n",fMapAfile.c_str());

  std::string fMapBDiffFile = getenv("GEBSYS");
  fMapBDiffFile.append("/libraries/TGRAnalysis/TPhosWall/BDiffcoeff.cal");//-322-8x8.xy");

  std::string line;
  std::ifstream infile;
  infile.open(fMapAfile.c_str());
  int junk,pixel;
  float gain,offset;
  while(getline(infile,line)) {
    if(line.length()<10) 
       continue;
    std::stringstream ss(line);
    ss >> junk; ss >> pixel;
    ss >> offset; ss >> gain;
    fAMap[pixel] = std::make_pair(offset,gain);
    //printf("%i\t%i\t%.02f\t%f\n",junk,pixel,offset,gain);
  }

  infile.close();
  infile.open(fMapBfile.c_str());
  while(getline(infile,line)) {
    if(line.length()<10) 
       continue;
    std::stringstream ss(line);
    ss >> junk; ss >> pixel;
    ss >> offset; ss >> gain;
    fBMap[pixel] = std::make_pair(offset,gain);
    //printf("%i\t%i\t%.02f\t%f\n",junk,pixel,offset,gain);
  }


  infile.close();
  infile.open(fMapBDiffFile.c_str());
  while(getline(infile,line)) {
    //if(line.length()<10) 
    //   continue;
    std::stringstream ss(line);
    ss >> pixel; ss >> junk;
    //ss >> offset; ss >> gain;
    fBDiffMap[pixel] = junk; //std::make_pair(offset,gain);
    //printf("%i\t%i\t%.02f\t%f\n",junk,pixel,offset,gain);
  }




  infile.close();
  infile.open(fMapCfile.c_str());
  while(getline(infile,line)) {
    if(line.length()<10) 
       continue;
    std::stringstream ss(line);
    ss >> junk; ss >> pixel;
    ss >> offset; ss >> gain;
    fCMap[pixel] = std::make_pair(offset,gain);
    //printf("%i\t%i\t%.02f\t%f\n",junk,pixel,offset,gain);
  }





  //printf("fAMap.size() = %i\n",fAMap.size());



//std::map<int,std::pair<float,float> > TPhosWall::fAMap;
//std::map<int,std::pair<float,float> > TPhosWall::fBMap;
//std::map<int,std::pair<float,float> > TPhosWall::fCMap;

};


Float_t TPhosWall::GetACal() {
   if(fLargestHit>=fNumberOfHits) 
      return 0.0; 
   if(!fCalMapsSet)
      return 0.0;
   float tmp = (Float_t)A(fLargestHit) + gRandom->Uniform(); 
   tmp = tmp* std::get<1>(fAMap[GetPixel()]) + std::get<0>(fAMap[GetPixel()]);
   return tmp;
}

Float_t TPhosWall::ACal(const int &i) {
   if(fLargestHit>=fNumberOfHits) 
      return 0.0; 
   if(!fCalMapsSet)
      return 0.0;
   float tmp = (Float_t)A(i) + gRandom->Uniform(); 
   tmp = tmp* std::get<1>(fAMap[Pixel(i)]) + std::get<0>(fAMap[Pixel(i)]);
   return tmp;
}


Float_t TPhosWall::GetBCal() {
   if(fLargestHit>=fNumberOfHits) { 
      //printf("fLargetHit = %i fTime.size() = %i\n",fLargestHit,fTime.size());
      return 0.0; 
   }
   if(!fCalMapsSet) {
      //printf("cal maps not yet set!!\n");
      return 0.0;
   }
   float tmp = (Float_t)B(fLargestHit) + gRandom->Uniform(); 
   tmp = tmp* std::get<1>(fBMap[GetPixel()]) + std::get<0>(fBMap[GetPixel()]);
   return tmp;
}


Int_t TPhosWall::BDiffCal(const int &i) {
   if(i>=fNumberOfHits) { 
      //printf("fLargetHit = %i fTime.size() = %i\n",fLargestHit,fTime.size());
      return 0; 
   }
   if(!fCalMapsSet) {
      //printf("cal maps not yet set!!\n");
      return 0;
   }
   //float tmp = (Float_t)fBCharge.at(fLargestHit) + gRandom->Uniform(); 
   if(fBDiffMap[Pixel(i)]==0)
      return 0;
   return fBDiffMap[Pixel(i)] + B(i);
}







Float_t TPhosWall::BCal(const int &i) {
   if(fLargestHit>=fNumberOfHits) 
      return 0.0; 
   if(!fCalMapsSet)
      return 0.0;
   float tmp = (Float_t)B(i) + gRandom->Uniform(); 
   tmp = tmp* std::get<1>(fBMap[Pixel(i)]) + std::get<0>(fBMap[Pixel(i)]);
   return tmp;
}

Float_t TPhosWall::GetCCal() {
   if(fLargestHit>=fNumberOfHits) 
      return 0.0; 
   if(!fCalMapsSet)
      return 0.0;
   float tmp = (Float_t)C(fLargestHit) + gRandom->Uniform(); 
   tmp = tmp* std::get<1>(fCMap[GetPixel()]) + std::get<0>(fCMap[GetPixel()]);
   return tmp;
}

Float_t TPhosWall::CCal(const int &i) {
   if(fLargestHit>=fNumberOfHits) 
      return 0.0; 
   if(!fCalMapsSet)
      return 0.0;
   float tmp = (Float_t)C(i) + gRandom->Uniform(); 
   tmp = tmp* std::get<1>(fCMap[Pixel(i)]) + std::get<0>(fCMap[Pixel(i)]);
   return tmp;
}

Float_t   TPhosWall::GetACalSmartSum(float res,float threshold) { 
  Float_t eng = 0.0;
  if(Size()<1)
     return eng;
  int lpixel = GetPixel();
  TVector3 lvec = *(FindWallPosition(lpixel));
  for(int y=0;y<Size();y++) {
    int cpixel = Pixel(y);
    if((cpixel/64)!=(lpixel/64))
       continue;
    TVector3 cvec = *(FindWallPosition(cpixel));
    float mag = (lvec-cvec).Mag();
    if(mag<res) {
      float tmp = ((float)A(y))*std::get<1>(fAMap[cpixel]) + std::get<0>(fAMap[cpixel]);
      if(tmp>threshold) {
        eng += tmp;
      }
    }
  }                                                          
  return eng;
}



Float_t   TPhosWall::GetBCalSmartSum(float res,float threshold) { 
  Float_t eng = 0.0;
  if(Size()<1)
     return eng;
  int lpixel = GetPixel();
  TVector3 lvec = *(FindWallPosition(lpixel));
  for(int y=0;y<Size();y++) {
    int cpixel = Pixel(y);
    if((cpixel/64)!=(lpixel/64))
       continue;
    TVector3 cvec = *(FindWallPosition(cpixel));
    float mag = (lvec-cvec).Mag();
    if(mag<res) { 
      float tmp = ((float)B(y))*std::get<1>(fBMap[cpixel]) + std::get<0>(fBMap[cpixel]);
      if (tmp>threshold) {
        eng += tmp;
      }
    }
  }                                                          
  return eng;
}


Float_t   TPhosWall::GetCCalSmartSum(float res,float threshold) { 
  Float_t eng = 0.0;
  if(Size()<1)
     return eng;
  int lpixel = GetPixel();
  TVector3 lvec = *(FindWallPosition(lpixel));
  for(int y=0;y<Size();y++) {
    int cpixel = Pixel(y);
    if((cpixel/64)!=(lpixel/64))
       continue;
    TVector3 cvec = *(FindWallPosition(cpixel));
    float mag = (lvec-cvec).Mag();
    if(mag<res) { 
      float tmp = ((float)C(y))*std::get<1>(fCMap[cpixel]) + std::get<0>(fCMap[cpixel]);
      if (tmp>threshold) {
        eng += tmp;
      }
    }
  }                                                          
  return eng;
}








