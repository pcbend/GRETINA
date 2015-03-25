#ifndef PWALLPOSITION
#define PWALLPOSITION

#include "Globals.h"

#include <TMath.h>

// average distance = 5.5 cm
//
// phi1 = phi
// phi2 = phi + 270
// phi3 = phi + 180
// phi4 = phi +  90

void SetPwallPositions() {
   double detoffset, phi;
   for(int i=0;i<4;i++) {

      if(i==0)      {detoffset = 0;    phi = TMath::DegToRad()*0;}
      else if(i==1) {detoffset = 64;   phi = TMath::DegToRad()*270;}
      else if(i==2) {detoffset = 64*2; phi = TMath::DegToRad()*180;}
      else if(i==3) {detoffset = 64*3; phi = TMath::DegToRad()*90;}
    
      pWallPositions[0  + detoffset] = new TVector3;  pWallPositions[0 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*37.141,TMath::DegToRad()*20.966 +  phi);
      pWallPositions[1  + detoffset] = new TVector3;  pWallPositions[1 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*32.895,TMath::DegToRad()*24.772 +  phi);
      pWallPositions[2  + detoffset] = new TVector3;  pWallPositions[2 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*28.445,TMath::DegToRad()*30.019 +  phi);
      pWallPositions[3  + detoffset] = new TVector3;  pWallPositions[3 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*23.908,TMath::DegToRad()*37.687 +  phi);
      pWallPositions[4  + detoffset] = new TVector3;  pWallPositions[4 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*19.653,TMath::DegToRad()*49.364 +  phi);
      pWallPositions[5  + detoffset] = new TVector3;  pWallPositions[5 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*16.391,TMath::DegToRad()*67.124 +  phi);   
      pWallPositions[6  + detoffset] = new TVector3;  pWallPositions[6 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*15.165,TMath::DegToRad()*-89.178 + phi);  
      pWallPositions[7  + detoffset] = new TVector3;  pWallPositions[7 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*16.580,TMath::DegToRad()*-65.538 + phi);  
                                                                
      pWallPositions[8  + detoffset] = new TVector3;  pWallPositions[8 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*40.971,TMath::DegToRad()*26.144 +  phi);   
      pWallPositions[9  + detoffset] = new TVector3;  pWallPositions[9 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*36.941,TMath::DegToRad()*30.589 +  phi);   
      pWallPositions[10 + detoffset] = new TVector3;  pWallPositions[10 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*32.748,TMath::DegToRad()*36.507 +  phi);
      pWallPositions[11 + detoffset] = new TVector3;  pWallPositions[11 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*28.546,TMath::DegToRad()*44.701 +  phi);
      pWallPositions[12 + detoffset] = new TVector3;  pWallPositions[12 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*24.730,TMath::DegToRad()*56.18  +  phi);
      pWallPositions[13 + detoffset] = new TVector3;  pWallPositions[13 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*21.943,TMath::DegToRad()*71.77 +   phi);
      pWallPositions[14 + detoffset] = new TVector3;  pWallPositions[14 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*20.940,TMath::DegToRad()*-89.358 + phi);
      pWallPositions[15 + detoffset] = new TVector3;  pWallPositions[15 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*22.100,TMath::DegToRad()*-70.448 + phi);
                                                                 
      pWallPositions[16 + detoffset] = new TVector3;  pWallPositions[16 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*45.272,TMath::DegToRad()*30.836  + phi);
      pWallPositions[17 + detoffset] = new TVector3;  pWallPositions[17 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*41.556,TMath::DegToRad()*35.713  + phi);
      pWallPositions[18 + detoffset] = new TVector3;  pWallPositions[18 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*37.721,TMath::DegToRad()*41.992  + phi);
      pWallPositions[19 + detoffset] = new TVector3;  pWallPositions[19 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*33.932,TMath::DegToRad()*50.277  + phi);
      pWallPositions[20 + detoffset] = new TVector3;  pWallPositions[20 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*30.574,TMath::DegToRad()*61.151  + phi);
      pWallPositions[21 + detoffset] = new TVector3;  pWallPositions[21 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*28.196,TMath::DegToRad()*74.846  + phi);
      pWallPositions[22 + detoffset] = new TVector3;  pWallPositions[22 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*27.361,TMath::DegToRad()*-89.472 + phi);
      pWallPositions[23 + detoffset] = new TVector3;  pWallPositions[23 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*28.328,TMath::DegToRad()*-73.722 + phi);
                                                                 
      pWallPositions[24 + detoffset] = new TVector3;  pWallPositions[24 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*50.035,TMath::DegToRad()*35.110  + phi);
      pWallPositions[25 + detoffset] = new TVector3;  pWallPositions[25 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*46.724,TMath::DegToRad()*40.255  + phi);
      pWallPositions[26 + detoffset] = new TVector3;  pWallPositions[26 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*43.330,TMath::DegToRad()*46.672  + phi);
      pWallPositions[27 + detoffset] = new TVector3;  pWallPositions[27 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*40.027,TMath::DegToRad()*54.797  + phi);
      pWallPositions[28 + detoffset] = new TVector3;  pWallPositions[28 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*37.149,TMath::DegToRad()*64.933  + phi);
      pWallPositions[29 + detoffset] = new TVector3;  pWallPositions[29 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*35.153,TMath::DegToRad()*77.050  + phi);
      pWallPositions[30 + detoffset] = new TVector3;  pWallPositions[30 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*34.462,TMath::DegToRad()*-89.552 + phi);
      pWallPositions[31 + detoffset] = new TVector3;  pWallPositions[31 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*35.263,TMath::DegToRad()*-76.075 + phi);
                                                                 
      pWallPositions[32 + detoffset] = new TVector3;  pWallPositions[32 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*55.164,TMath::DegToRad()*38.979  + phi);
      pWallPositions[33 + detoffset] = new TVector3;  pWallPositions[33 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*52.326,TMath::DegToRad()*44.260  + phi);
      pWallPositions[34 + detoffset] = new TVector3;  pWallPositions[34 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*49.446,TMath::DegToRad()*50.663  + phi);
      pWallPositions[35 + detoffset] = new TVector3;  pWallPositions[35 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*46.672,TMath::DegToRad()*58.492  + phi);
      pWallPositions[36 + detoffset] = new TVector3;  pWallPositions[36 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*44.293,TMath::DegToRad()*67.884  + phi);
      pWallPositions[37 + detoffset] = new TVector3;  pWallPositions[37 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*42.667,TMath::DegToRad()*78.701  + phi);
      pWallPositions[38 + detoffset] = new TVector3;  pWallPositions[38 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*42.109,TMath::DegToRad()*-89.611 + phi);
      pWallPositions[39 + detoffset] = new TVector3;  pWallPositions[39 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*42.756,TMath::DegToRad()*-77.843 + phi);
                                                                 
      pWallPositions[40 + detoffset] = new TVector3;  pWallPositions[40 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*60.528,TMath::DegToRad()*42.468  + phi);
      pWallPositions[41 + detoffset] = new TVector3;  pWallPositions[41 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*58.205,TMath::DegToRad()*47.786  + phi);
      pWallPositions[42 + detoffset] = new TVector3;  pWallPositions[42 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*55.871,TMath::DegToRad()*54.074  + phi);
      pWallPositions[43 + detoffset] = new TVector3;  pWallPositions[43 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*53.651,TMath::DegToRad()*61.546  + phi);
      pWallPositions[44 + detoffset] = new TVector3;  pWallPositions[44 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*51.772,TMath::DegToRad()*70.238  + phi);
      pWallPositions[45 + detoffset] = new TVector3;  pWallPositions[45 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*50.503,TMath::DegToRad()*79.983  + phi);
      pWallPositions[46 + detoffset] = new TVector3;  pWallPositions[46 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*50.071,TMath::DegToRad()*-89.656 + phi);
      pWallPositions[47 + detoffset] = new TVector3;  pWallPositions[47 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*50.572,TMath::DegToRad()*-79.217 + phi);
                                                                 
      pWallPositions[48 + detoffset] = new TVector3;  pWallPositions[48 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*65.983,TMath::DegToRad()*45.606  + phi);
      pWallPositions[49 + detoffset] = new TVector3;  pWallPositions[49 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*64.180,TMath::DegToRad()*50.890  + phi);
      pWallPositions[50 + detoffset] = new TVector3;  pWallPositions[50 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*62.390,TMath::DegToRad()*57.005  + phi);
      pWallPositions[51 + detoffset] = new TVector3;  pWallPositions[51 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*60.711,TMath::DegToRad()*64.098  + phi);
      pWallPositions[52 + detoffset] = new TVector3;  pWallPositions[52 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*59.308,TMath::DegToRad()*72.154  + phi);
      pWallPositions[53 + detoffset] = new TVector3;  pWallPositions[53 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*58.371,TMath::DegToRad()*81.006  + phi);
      pWallPositions[54 + detoffset] = new TVector3;  pWallPositions[54 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*58.054,TMath::DegToRad()*-89.692 + phi);
      pWallPositions[55 + detoffset] = new TVector3;  pWallPositions[55 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*58.422,TMath::DegToRad()*-80.315 + phi);
                                                                 
      pWallPositions[56 + detoffset] = new TVector3;  pWallPositions[56 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*71.343,TMath::DegToRad()*48.456  + phi);
      pWallPositions[57 + detoffset] = new TVector3;  pWallPositions[57 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*70.127,TMath::DegToRad()*53.656  + phi);
      pWallPositions[58 + detoffset] = new TVector3;  pWallPositions[58 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*68.848,TMath::DegToRad()*59.560  + phi);
      pWallPositions[59 + detoffset] = new TVector3;  pWallPositions[59 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*67.665,TMath::DegToRad()*66.274  + phi);
      pWallPositions[60 + detoffset] = new TVector3;  pWallPositions[60 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*66.691,TMath::DegToRad()*73.755  + phi);
      pWallPositions[61 + detoffset] = new TVector3;  pWallPositions[61 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*66.047,TMath::DegToRad()*81.484  + phi);
      pWallPositions[62 + detoffset] = new TVector3;  pWallPositions[62 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*65.831,TMath::DegToRad()*-89.721 + phi);
      pWallPositions[63 + detoffset] = new TVector3;  pWallPositions[63 + detoffset]->SetMagThetaPhi(55.0,TMath::DegToRad()*66.082,TMath::DegToRad()*-81.219 + phi);
   }
}





#endif


