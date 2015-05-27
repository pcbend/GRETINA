
#include <cstdio>
#include <TMath.h>

#include <TGretina.h>
#include <TGretinaHit.h>
#include <TPhosWall.h>
#include <TLaBr.h>
#include <TLaBrHit.h>

#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TH1.h>
#include <TH2.h>
#include <TList.h>

#include <TNucleus.h>
#include <TKinematics.h>
#include <TChannel.h>


TList *labr_list  = new TList;
TList *hpge_list  = new TList;
TList *pwall_list = new TList;
TList *misc_list  = new TList;



void PrintHelp();

int main(int argc,char **argv) {
  if(argc<2) {
    PrintHelp();
    return 1;
  }

  //TFile g(argv[argc-1]);
  //TCutG *proton       = (TCutG*)g.Get("proton"); 
  //TCutG *alpha        = (TCutG*)g.Get("alpha"); 
  //TCutG *betaparticle = (TCutG*)g.Get("beta"); 

 
  TFile f(argv[1]);
  TChain *chain = new TChain("Data");
  for(int y=1;y<argc;y++) {
    //printf("%i\t%s\n",y,argv[y]);
    chain->Add(argv[y]);
  }
  
  TGretina  *gretina  = 0;
  TPhosWall *phoswall = 0;
  TLaBr     *labr     = 0;
  chain->SetBranchAddress("TGretina",&gretina);
  chain->SetBranchAddress("TPhosWall",&phoswall);
  chain->SetBranchAddress("TLaBr",&labr);

  int nentries = chain->GetEntries();
  
  //TNucleus *beam     = new TNucleus("138Ba");
  //TNucleus *target   = new TNucleus("13C");
  //TNucleus *recoil   = new TNucleus("139Ba");
  //TNucleus *ejectile = new TNucleus("12C");
  
  //TKinematics *kin = new TKinematics(beam,target,recoil,ejectile,27.0,7.0);
  
  int stop = 0;
  int x;
  for(x=0;x<nentries;x++) {
  
    if((x%10000)==0) {
       printf("  On entry %i / %i              \r",x,nentries);
    }
    chain->GetEntry(x);

    std::string hname;
    TH1* hist;
    TH2* mat;

    double beta = 0.08;

    for(int g=0;g<gretina->NumberOfGretinaHits();g++) { 
      TGretinaHit *ghit = gretina->GetHit(g);

      hname.assign("CryId_LabEngery");
      mat = (TH2F*)hpge_list->FindObject(hname.c_str());
      if(!mat) {
        mat = new TH2F(hname.c_str(),hname.c_str(),200,0,200,4000,0,4000);
        hpge_list->Add(mat);
      }
      mat->Fill(ghit->GetCrystalId(),ghit->GetCoreEnergy());

      hname.assign("LabEnergy_Total");
      hist = (TH1F*)hpge_list->FindObject(hname.c_str());
      if(!hist) {
        hist = new TH1F(hname.c_str(),hname.c_str(),8000,0,4000);
        hpge_list->Add(hist);
      }
      hist->Fill(ghit->GetCoreEnergy());

      hname.assign("CryId_DopplerEngery");
      mat = (TH2F*)hpge_list->FindObject(hname.c_str());
      if(!mat) {
        mat = new TH2F(hname.c_str(),hname.c_str(),200,0,200,4000,0,4000);
        hpge_list->Add(mat);
      }
      mat->Fill(ghit->GetCrystalId(),ghit->GetDoppler(beta));
 
      hname.assign("DopplerEngery_Total");
      hist = (TH1F*)hpge_list->FindObject(hname.c_str());
      if(!hist) {
        hist = new TH1F(hname.c_str(),hname.c_str(),8000,0,4000);
        hpge_list->Add(hist);
      }
      hist->Fill(ghit->GetDoppler(beta));
    
      for(int g2=0;g2<gretina->NumberOfGretinaHits();g2++) { 
         if(g2==g)
           continue;
         TGretinaHit *ghit2 = gretina->GetHit(g);

         hname.assign("Gamma_Gamma_LabEnergy");
         mat = (TH2F*)hpge_list->FindObject(hname.c_str());
         if(!mat) {
           mat = new TH2F(hname.c_str(),hname.c_str(),4000,0,4000,4000,0,4000);
           hpge_list->Add(mat);
         }
         mat->Fill(ghit->GetCoreEnergy(),ghit2->GetCoreEnergy());

         hname.assign("Gamma_Gamma_DopplerEnergy");
         mat = (TH2F*)hpge_list->FindObject(hname.c_str());
         if(!mat) {
           mat = new TH2F(hname.c_str(),hname.c_str(),4000,0,4000,4000,0,4000);
           hpge_list->Add(mat);
         }
         mat->Fill(ghit->GetDoppler(beta),ghit2->GetDoppler(beta));
         
         hname.assign("Gamma_Gamma_DeltaTime");
         hist = (TH1F*)hpge_list->FindObject(hname.c_str());
         if(!hist) {
           hist = new TH1F(hname.c_str(),hname.c_str(),2000,-1000,1000);
           hpge_list->Add(hist);
         }
         hist->Fill(ghit->GetTime()-ghit2->GetTime());

      }

      for(int l=0;l<labr->Size();l++) { 
         TLaBrHit *lhit = labr->GetHit(l);
 
         hname.assign("Gretina_LaBr_Time");
         hist = (TH1F*)misc_list->FindObject(hname.c_str());
         if(!hist) {
           hist = new TH1F(hname.c_str(),hname.c_str(),2000,-1000,1000);
           misc_list->Add(hist);
         }
         hist->Fill(ghit->GetTime()-lhit->GetTime());
   
         hname.assign("Gretina_LaBr_LabEnergy");
         mat = (TH2F*)misc_list->FindObject(hname.c_str());
         if(!mat) {
           mat = new TH2F(hname.c_str(),hname.c_str(),4000,0,4000,4000,0,4000);
           misc_list->Add(mat);
         }
         mat->Fill(ghit->GetCoreEnergy(),lhit->GetEnergy());

      }



    }

    for(int a=0;a<gretina->NumberOfAddbackHits();a++) {
      TGretinaHit *ahit = gretina->GetAddbackHit(a);
      
      hname.assign("AddBack_LabEnergy_Total");
      hist = (TH1F*)hpge_list->FindObject(hname.c_str());
      if(!hist) {
        hist = new TH1F(hname.c_str(),hname.c_str(),8000,0,4000);
        hpge_list->Add(hist);
      }
      hist->Fill(ahit->GetCoreEnergy());

      hname.assign("AddBack_DopplerEnergy_Total");
      hist = (TH1F*)hpge_list->FindObject(hname.c_str());
      if(!hist) {
        hist = new TH1F(hname.c_str(),hname.c_str(),8000,0,4000);
        hpge_list->Add(hist);
      }
      hist->Fill(ahit->GetDoppler(beta));

      for(int a2=0;a2<gretina->NumberOfAddbackHits();a2++) {
         if(a2 == a) 
           continue;
         TGretinaHit *ahit2 = gretina->GetAddbackHit(a);

         hname.assign("Gamma_Gamma_LabEnergy_Addback");
         mat = (TH2F*)hpge_list->FindObject(hname.c_str());
         if(!mat) {
           mat = new TH2F(hname.c_str(),hname.c_str(),4000,0,4000,4000,0,4000);
           hpge_list->Add(mat);
         }
         mat->Fill(ahit->GetCoreEnergy(),ahit2->GetCoreEnergy());

         hname.assign("Gamma_Gamma_DopplerEnergy_Addback");
         mat = (TH2F*)hpge_list->FindObject(hname.c_str());
         if(!mat) {
           mat = new TH2F(hname.c_str(),hname.c_str(),4000,0,4000,4000,0,4000);
           hpge_list->Add(mat);
         }
         mat->Fill(ahit->GetDoppler(beta),ahit2->GetDoppler(beta));

      }


    }

    for(int l=0;l<labr->Size();l++) { 
      TLaBrHit *lhit = labr->GetHit(l);
      hname.assign("LabrId_LabCharge");
      mat = (TH2F*)labr_list->FindObject(hname.c_str());
      if(!mat) {
        mat = new TH2F(hname.c_str(),hname.c_str(),20,0,20,4000,0,8000);
        labr_list->Add(mat);
      }
      mat->Fill(lhit->GetCrystalId(),lhit->GetCharge());
 
      hname.assign("LabrId_LabEnergy");
      mat = (TH2F*)labr_list->FindObject(hname.c_str());
      if(!mat) {
        mat = new TH2F(hname.c_str(),hname.c_str(),20,0,20,4000,0,8000);
        labr_list->Add(mat);
      }
      mat->Fill(lhit->GetCrystalId(),lhit->GetEnergy());

      hname.assign("LabrId_TDC");
      mat = (TH2F*)labr_list->FindObject(hname.c_str());
      if(!mat) {
        mat = new TH2F(hname.c_str(),hname.c_str(),20,0,20,4000,0,8000);
        labr_list->Add(mat);
      }
      mat->Fill(lhit->GetCrystalId(),lhit->GetTDC());

      hname.assign("Labr_LabEnergy_TDC");
      mat = (TH2F*)labr_list->FindObject(hname.c_str());
      if(!mat) {
        mat = new TH2F(hname.c_str(),hname.c_str(),4000,0,4000,4000,0,8000);
        labr_list->Add(mat);
      }
      mat->Fill(lhit->GetTDC(),lhit->GetEnergy());


    }
    
    for(int p=0;p<phoswall->Size();p++) { 
      
      hname.assign("Pixel_A");
      mat = (TH2F*)pwall_list->FindObject(hname.c_str());
      if(!mat) {
        mat = new TH2F(hname.c_str(),hname.c_str(),300,0,300,4000,0,8000);
        pwall_list->Add(mat);
      }
      mat->Fill(phoswall->Pixel(p),phoswall->A(p));

      hname.assign("Pixel_B");
      mat = (TH2F*)pwall_list->FindObject(hname.c_str());
      if(!mat) {
        mat = new TH2F(hname.c_str(),hname.c_str(),300,0,300,4000,0,8000);
        pwall_list->Add(mat);
      }
      mat->Fill(phoswall->Pixel(p),phoswall->B(p));

      hname.assign("Pixel_C");
      mat = (TH2F*)pwall_list->FindObject(hname.c_str());
      if(!mat) {
        mat = new TH2F(hname.c_str(),hname.c_str(),300,0,300,4000,0,8000);
        pwall_list->Add(mat);
      }
      mat->Fill(phoswall->Pixel(p),phoswall->C(p));

      hname.assign("Pixel_Time");
      mat = (TH2F*)pwall_list->FindObject(hname.c_str());
      if(!mat) {
        mat = new TH2F(hname.c_str(),hname.c_str(),300,0,300,4000,0,8000);
        pwall_list->Add(mat);
      }
      mat->Fill(phoswall->Pixel(p),phoswall->Time(p));

    }
     




  }
    
  printf("  On entry %i / %i              \n",x,nentries);

  labr_list->Sort();
  hpge_list->Sort();
  pwall_list->Sort();
  misc_list->Sort();

  TFile outfile("HistogramsOut.root","recreate");
  outfile.mkdir("LaBr"); 
  outfile.cd("LaBr");
  labr_list->Write(); 
  outfile.cd("/");

  outfile.mkdir("HPGe"); 
  outfile.cd("HPGe");
  hpge_list->Write(); 
  outfile.cd("/");
  
  outfile.mkdir("PWall"); 
  outfile.cd("PWall");
  pwall_list->Write(); 
  outfile.cd("/");

  outfile.mkdir("misc"); 
  outfile.cd("misc");
  misc_list->Write(); 
  outfile.cd("/");

  outfile.Close();

  return 0;
}




void PrintHelp() {  
  printf("help.\n");
}






