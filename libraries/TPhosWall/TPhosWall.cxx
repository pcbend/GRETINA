

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


int TPhosWall::fHitPatternCounter=0;


bool TPhosWall::fCalMapsSet = false;

std::map<int,std::pair<float,float> > TPhosWall::fAMap;
std::map<int,std::pair<float,float> > TPhosWall::fBMap;
std::map<int,std::pair<float,float> > TPhosWall::fCMap;





TPhosWall::TPhosWall() { 
   Clear(); 
   if(!fPositionsSet)
      SetWallPositions();
   if(!fCalMapsSet)
      SetCalMaps();

}

TPhosWall::~TPhosWall() {  }

void TPhosWall::AddPWHit(PWFragment &frag) {
   Clear();
   fTimeStamp  = frag.GetTimeStamp();
   fLargestHit = frag.GetLargestNumber();

   for(unsigned int x=0;x<frag.GetNumberOfHits();x++) {

      fACharge.push_back(frag.GetSegment(x).A());
      fBCharge.push_back(frag.GetSegment(x).B());
      fCCharge.push_back(frag.GetSegment(x).C());
      fPixel.push_back(frag.GetSegment(x).Pixel());
      fTime.push_back(frag.GetSegment(x).Time());
      if(fPixel.back()>-1&&fPixel.back()<256) {
         //printf("here1,  %i  \n",fPixel.back());
         //printf("here2,  0x%08x  \n",fWallPositions[fPixel.back()]);
         fPosition.push_back(*fWallPositions[fPixel.back()]);
      }
      else
         fPosition.push_back(frag.GetSegment(x).Position());
   }
}

void TPhosWall::SetWeightedPosition() {
  if(fACharge.size()<=fLargestHit)
     return;
  float ChargeSum=0.0;
  float MaxPixelDistance = 20.0;
  for(int i=0;i<fACharge.size();i++) {
    if((fPosition.at(fLargestHit)-fPosition.at(i)).Mag()<MaxPixelDistance) 
      ChargeSum += (float)fACharge.at(i);
  }
  fWeightedPosition =  fPosition.at(fLargestHit);
  fWeightedPosition *= (((float)fACharge.at(fLargestHit))/ChargeSum);
  double x =0.0;
  double y =0.0;
  double z =0.0;
 
  fMultiplicity = 1;

  
  for(int i=0;i<fPosition.size();i++) {
    //printf("Mag: %.02f\tAChg = %i\n",(fPosition.at(fLargestHit)-fPosition.at(i)).Mag(),fACharge.at(i));
    if(i==fLargestHit) {
       //printf(DGREEN);
       //printf("[%02i][%03i]  ",i,fPixel.at(i)); fPosition.at(i).Print();
       //printf(RESET_COLOR);
       continue;
    }
    //printf("[%02i][%03i]  ",i,fPixel.at(i)); fPosition.at(i).Print();
    if( ((fPosition.at(fLargestHit)-fPosition.at(i)).Mag()<MaxPixelDistance) &&
        ((fPixel.at(fLargestHit)/64) == (fPixel.at(i)/64)) ) {
      //fMultiplicity++;
      TVector3 temp = (((float)fACharge.at(i))/ChargeSum)*(fPosition.at(i));
      fWeightedPosition += temp;
    } else {
      fMultiplicity++;
    }
  }
  //printf("\t[%02i]",fMultiplicity);fWeightedPosition.Print();
  //printf("----------------------------------\n");

}

void TPhosWall::Print(Option_t *opt) {
  
  for(int i=0;i<fPosition.size();i++) {
    if(i==fLargestHit) {
       printf(DGREEN);
       printf("Mag: %.02f\tAChg = %i\n",(fPosition.at(fLargestHit)-fPosition.at(i)).Mag(),fACharge.at(i));
       printf("[%02i][%03i]  ",i,fPixel.at(i)); fPosition.at(i).Print();
       printf(RESET_COLOR);
       continue;
    }
    printf("Mag: %.02f\tAChg = %i\n",(fPosition.at(fLargestHit)-fPosition.at(i)).Mag(),fACharge.at(i));
    printf("[%02i][%03i]  ",i,fPixel.at(i)); fPosition.at(i).Print();
  }
  printf(DYELLOW);
  printf("\t[%02i]",fMultiplicity);fWeightedPosition.Print();
  printf(RESET_COLOR);
  printf("----------------------------------\n");
}

void TPhosWall::Clear(Option_t *opt) {

   fTimeStamp  = 0;
   fLargestHit = 0;
   fWeightedPosition.SetXYZ(0,0,1);

   fMultiplicity = 0;

   fPosition.clear();
   fACharge.clear();
   fBCharge.clear();
   fCCharge.clear();
   fPixel.clear();
   fTime.clear();

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
  for(int x=0;x<fPixel.size();x++) {
    int hist = fPixel.at(x)/64;
    int col  = ((fPixel.at(x)-(hist*64))%8);
    //if(strcmp(opt,"order")==0) {
    //  printf("reordering happening.\n");
    //  if(col==7) col=5;
    //  else if(col==6) col=7;
    //  else if(col==5) col=6;
    //}
    int row  = (fPixel.at(x)-(hist*64))/8;
    if(strcmp(opt,"order")) {
      if(row==7) row=6;
      else if(row==6) row=7;
    }
    hitpat[hist].Fill(row,col,fACharge.at(x));
    printf("\t\tPixel[%03i]  r[%02i] c[%02i]   %i\n",fPixel.at(x),row,col,fACharge.at(x));
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
  mother->cd(3); hitpat[1].DrawCopy("colz");
  gPad->SetGrid();
  if(exef.size()) gPad->AddExec(exef.c_str(),exex.c_str());
  mother->cd(4); hitpat[2].DrawCopy("colz");
  gPad->SetGrid();
  if(exef.size()) gPad->AddExec(exef.c_str(),exex.c_str());
  mother->cd(2); hitpat[3].DrawCopy("colz");
  gPad->SetGrid();
  if(exef.size()) gPad->AddExec(exef.c_str(),exex.c_str());
  gPad->Modified();
  gPad->Update();
  return;
}


void TPhosWall::DrawXY(Option_t *opt) {
  TH2I hitpattern("hitpattern","PWFragmet XY Hit Pattern",
                   116,-58,58,116,-58,58);
  for(int i=0;i<fPosition.size();i++) {
     hitpattern.Fill(fPosition.at(i).X(),
                     fPosition.at(i).Y(),
                     fACharge.at(i));
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

void TPhosWall::SetWallPositions() {
   printf("Setting PhosWall positions.\n");
   int detoffset;
   double phi;
   for(int i=0;i<4;i++) {
      if(i==0)      {detoffset = 0;    phi = TMath::DegToRad()*0;}
      else if(i==1) {detoffset = 64;   phi = TMath::DegToRad()*270;}
      else if(i==2) {detoffset = 64*2; phi = TMath::DegToRad()*180;}
      else if(i==3) {detoffset = 64*3; phi = TMath::DegToRad()*90;}

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
  fMapAfile.append("/libraries/TPhosWall/Acoeff-322-8x8.xy");
  fMapBfile.append("/libraries/TPhosWall/Bcoeff-322-8x8.xy");
  fMapCfile.append("/libraries/TPhosWall/Ccoeff-322-8x8.xy");
  //printf("fMapAfile = %s\n",fMapAfile.c_str());

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
    ss >> offset; ss >> gain*100;
    fBMap[pixel] = std::make_pair(offset,gain);
    //printf("%i\t%i\t%.02f\t%f\n",junk,pixel,offset,gain);
  }

  infile.close();
  infile.open(fMapCfile.c_str());
  while(getline(infile,line)) {
    if(line.length()<10) 
       continue;
    std::stringstream ss(line);
    ss >> junk; ss >> pixel;
    ss >> offset; ss >> gain*100;
    fCMap[pixel] = std::make_pair(offset,gain);
    //printf("%i\t%i\t%.02f\t%f\n",junk,pixel,offset,gain);
  }

  //printf("fAMap.size() = %i\n",fAMap.size());



//std::map<int,std::pair<float,float> > TPhosWall::fAMap;
//std::map<int,std::pair<float,float> > TPhosWall::fBMap;
//std::map<int,std::pair<float,float> > TPhosWall::fCMap;

};


Float_t TPhosWall::GetACal() {
   if(fLargestHit>=fTime.size()) 
      return 0.0; 
   if(!fCalMapsSet)
      return 0.0;
   float tmp = (Float_t)fACharge.at(fLargestHit) + gRandom->Uniform(); 
   tmp = tmp* std::get<1>(fAMap[fLargestHit]) + std::get<0>(fAMap[fLargestHit]);
   return tmp;
}


Float_t TPhosWall::GetBCal() {
   if(fLargestHit>=fTime.size()) 
      return 0.0; 
   if(!fCalMapsSet)
      return 0.0;
   float tmp = (Float_t)fBCharge.at(fLargestHit) + gRandom->Uniform(); 
   tmp = tmp* std::get<1>(fBMap[fLargestHit]) + std::get<0>(fBMap[fLargestHit]);
   return tmp;
}


Float_t TPhosWall::GetCCal() {
   if(fLargestHit>=fTime.size()) 
      return 0.0; 
   if(!fCalMapsSet)
      return 0.0;
   float tmp = (Float_t)fCCharge.at(fLargestHit) + gRandom->Uniform(); 
   tmp = tmp* std::get<1>(fCMap[fLargestHit]) + std::get<0>(fCMap[fLargestHit]);
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
      float tmp = A(y)*std::get<1>(fAMap[y]) + std::get<0>(fAMap[y]);
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
      float tmp = B(y)*std::get<1>(fBMap[y]) + std::get<0>(fBMap[y]);
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
      float tmp = C(y)*std::get<1>(fCMap[y]) + std::get<0>(fCMap[y]);
      if (tmp>threshold) {
        eng += tmp;
      }
    }
  }                                                          
  return eng;
}








