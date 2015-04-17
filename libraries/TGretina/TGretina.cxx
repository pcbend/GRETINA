

#include <TGretina.h>



ClassImp(TGretina)


TGretina::TGretina() { 
   if(!fCRMATSet)
     SetCRMAT();
   Clear(); 
}

TGretina::~TGretina() {  }


Float_t TGretina::crmat[31][4][4][4];
bool TGretina::fCRMATSet = false;



void TGretina::BuildAddBack() {
  if(gretina_hits.size()==0)
     return;
  addback_hits.clear();
  addback_hits.push_back(gretina_hits.at(0));
  if(gretina_hits.size()==1) 
    return;


  for(int x=1;x<gretina_hits.size();x++) {
    bool used = false;
    for(int y=0;y<addback_hits.size();y++) {
      if(addback_hits.at(y).CheckAddback(gretina_hits.at(x))) {
        used = true;
        addback_hits.at(y) += gretina_hits.at(x);
      }
    }
    if(!used)
       addback_hits.push_back(gretina_hits.at(x));
  }
};

void TGretina::Print(Option_t *opt) { }

void TGretina::Clear(Option_t *opt) {
   gretina_hits.clear();
   addback_hits.clear();
   //tracked_hits.clear();
}



TVector3 TGretina::FindPosition(int cryId,Float_t x,Float_t y,Float_t z) {
  //Int_t detectorPosition = ((cryId & 0xfffc)>>2);
  //Int_t crystalNumber = (cryId & 0x0003);
  //detectorPosition -= 1;
  Int_t detectorPosition = cryId/4;
  Int_t crystalNumber    = cryId%4;
  /* x,y,z need to be in cm to work properly. Depending on the 
     source of the mapping, you might need to convert from mm 
     (if you read from crmat.linux). */
  double xl,yl,zl;

  xl = ( (crmat[detectorPosition][crystalNumber][0][0] * x) +
         (crmat[detectorPosition][crystalNumber][0][1] * y) +
         (crmat[detectorPosition][crystalNumber][0][2] * z) +
         (crmat[detectorPosition][crystalNumber][0][3]) );

  yl = ( (crmat[detectorPosition][crystalNumber][1][0] * x) +
         (crmat[detectorPosition][crystalNumber][1][1] * y) +
         (crmat[detectorPosition][crystalNumber][1][2] * z) +
         (crmat[detectorPosition][crystalNumber][1][3]) );

  zl = ( (crmat[detectorPosition][crystalNumber][2][0] * x) +
         (crmat[detectorPosition][crystalNumber][2][1] * y) +
         (crmat[detectorPosition][crystalNumber][2][2] * z) +
         (crmat[detectorPosition][crystalNumber][2][3]) );
   TVector3 pos;
   pos.SetXYZ(xl,yl,zl);
   return pos;
}


void TGretina::SetCRMAT() {

   FILE *fp;

   std::string temp = getenv("GEBSYS");
   temp.append("/libraries/TGretina/crmat.dat");

   const char *fn = temp.c_str();

   float f1, f2, f3, f4;
   int pos, xtal;
   int nn = 0;
   char *st, str[256];

   fp = fopen64(fn, "r");
   if (fp == NULL) {
      printf("Could not open \"%s\".\n", fn);
      exit(1);
   }
   printf("\"%s\" open....", fn);

   /* Read values. */
   nn = 0;
   st = fgets(str, 256, fp);
   while (st != NULL) {
      if (str[0] == 35) {
         /* '#' comment line, do nothing */
      } else if (str[0] == 59) {
         /* ';' comment line, do nothing */
      } else if (str[0] == 10) {
         /* Empty line, do nothing */
      } else {
         sscanf(str, "%i %i", &pos, &xtal);
         for (int i=0; i<4; i++) {
            st = fgets(str, 256, fp);
            sscanf(str, "%f %f %f %f", &f1, &f2, &f3, &f4);
            crmat[pos-1][xtal][i][0] = f1;
            crmat[pos-1][xtal][i][1] = f2;
            crmat[pos-1][xtal][i][2] = f3;
            crmat[pos-1][xtal][i][3] = f4;
         }
         nn++;
      }

      /* Attempt to read the next line. */
      st = fgets(str, 256, fp);
   }
   fCRMATSet = true;
   printf("Read %i rotation matrix coefficients.\n", nn);

   /* Done! */
}
