
#include "Globals.h"

#include <TClass.h>
#include <TPaveStats.h>
#include <TList.h>
#include <TText.h>
#include <TLatex.h>
#include <TH1.h>
#include <Buttons.h>
#include <KeySymbols.h> 
#include <TVirtualX.h>
#include <TROOT.h>
#include <TFrame.h>
#include <TF1.h>
#include <TGraph.h>

#include <TContextMenu.h>

#include "GCanvas.h"
#include "GROOTGuiFactory.h"

#include <iostream>
#include <TMath.h>

#ifndef kArrowKeyPress
#define kArrowKeyPress 25
#define kArrowKeyRelease 26
#endif

int GCanvas::lastx = 0;
int GCanvas::lasty = 0;

GCanvas::GCanvas(Bool_t build)
        :TCanvas(build)  {  
   GCanvasInit();
}


GCanvas::GCanvas(const char* name, const char* title, Int_t form)
        :TCanvas(name,title,form) { 
   GCanvasInit();

}


GCanvas::GCanvas(const char* name, const char* title, Int_t ww, Int_t wh)
        :TCanvas(name,title,ww,wh) { 
   GCanvasInit();

}


GCanvas::GCanvas(const char* name, Int_t ww, Int_t wh, Int_t winid)
        :TCanvas(name,ww,wh,winid) { 
  // this constructor is used to create an embedded canvas
  // I see no reason for us to support this here.  pcb.
  GCanvasInit();
}


GCanvas::GCanvas(const char* name, const char* title, Int_t wtopx, Int_t wtopy, Int_t ww, Int_t wh)
        :TCanvas(name,title,wtopx,wtopy,ww,wh) { 
   GCanvasInit();
}


GCanvas::~GCanvas() {
   //TCanvas::~TCanvas();           
}

void GCanvas::GCanvasInit() {
   printf("GCanvasInit called.\n");
   // ok, to interact with the default TGWindow
   // stuff from the root gui we need our own GRootCanvas.  
   // We make this using GROOTGuiFactory, which replaces the
   // TRootGuiFactory used in the creation of some of the 
   // default gui's (canvas,browser,etc).  
   fStatsDisplayed = true;
   fMarkerMode     = false;

   //if(gVirtualX->InheritsFrom("TGX11")) {
   //    printf("\tusing x11-like graphical interface.\n");
   //}
   //this->SetCrosshair(true);
}

void GCanvas::AddMarker(int x,int y,int dim) {
  GMarker *mark = new GMarker();
  mark->x = x;
  mark->y = y;
  if(dim==1) {
    mark->localx = gPad->AbsPixeltoX(x);
    mark->linex = new TLine(mark->localx,GetUymin(),mark->localx,GetUymax());
    mark->linex->SetLineColor(kRed);
    mark->linex->Draw();
  } else if (dim==2) {
    mark->localx = gPad->AbsPixeltoX(x);
    mark->localy = gPad->AbsPixeltoX(y);
    mark->linex = new TLine(mark->localx,GetUymin(),mark->localx,GetUymax());
    mark->linex->SetLineColor(kRed);
    mark->liney = new TLine(GetUxmin(),mark->localy,GetUxmax(),mark->localy);
    mark->liney->SetLineColor(kRed);
    mark->linex->Draw();
    mark->liney->Draw();
  }
  fMarkers.push_back(mark);
  //printf("MarkerAdded %i | %i",x,y);
}

void GCanvas::RemoveMarker() {
  if(fMarkers.size()<1)
    return;
  if(fMarkers.at(0))
     delete fMarkers.at(0);
  //printf("Marker %i Removed\n");
  fMarkers.erase(fMarkers.begin());
  return;
}

GCanvas *GCanvas::MakeDefCanvas() { 

  // Static function to build a default canvas.

  const char *defcanvas = gROOT->GetDefCanvasName();
  char *cdef;

  TList *lc = (TList*)gROOT->GetListOfCanvases();
  if (lc->FindObject(defcanvas)) {
    Int_t n = lc->GetSize() + 1;
    cdef = new char[strlen(defcanvas)+15];
    do {
      strlcpy(cdef,Form("%s_n%d", defcanvas, n++),strlen(defcanvas)+15);
    } while (lc->FindObject(cdef));
  } else
    cdef = StrDup(Form("%s",defcanvas));
  GCanvas *c = new GCanvas(cdef, cdef, 1);
  //printf("GCanvas::MakeDefCanvas"," created default GCanvas with name %s",cdef);
  delete [] cdef;
  return c;
};

//void GCanvas::ProcessEvent(Int_t event,Int_t x,Int_t y,TObject *obj) {
//   printf("{GCanvas} ProcessEvent:\n");
//   printf("\tevent: \t0x%08x\n",event);
//   printf("\tobject:\t0x%08x\n",obj);
//   printf("\tx:     \t0x%i\n",x);
//   printf("\ty:     \t0x%i\n",y);
//}

//void GCanvas::ExecuteEvent(Int_t event,Int_t x,Int_t y) { 
//  printf("exc event called.\n");
//}


void GCanvas::HandleInput(Int_t event,Int_t x,Int_t y) {
  //If the below switch breaks. You need to upgrade your version of ROOT
  //Version 5.34.24 works.

  //if(this->GetPadSave()!=gPad)
  //   gPad = this->GetPadSave();


  bool used = false;
  switch(event) {
    case 0x00000001:
      used = HandleMousePress(event,x,y);
      break;
  };
  if(!used)
    TCanvas::HandleInput((EEventType)event,x,y);
  


  return;
}


void GCanvas::UpdateStatsInfo(int x, int y) {
   TIter next(this->GetListOfPrimitives());
   TObject *obj;
   while((obj=next())) {
      if(obj->InheritsFrom("TH1")) {
         ((TH1*)obj)->SetBit(TH1::kNoStats);
         printf("found : %s\n",obj->GetName());
         TPaveStats *st = (TPaveStats*)((TH1*)obj)->GetListOfFunctions()->FindObject("stats");
         st->GetListOfLines()->Delete();
         st->AddText(Form("X      %i",x));
         st->AddText(Form("Counts %i",y));
         
         //st->Paint();
         //gPad->Modified();
         //gPad->Update();
      }
   }
}

//void GCanvas::HandleKeyPress(int event,int x,int key,TObject *obj) {
//
//}

void GCanvas::Draw(Option_t *opt) {
   printf("GCanvas Draw was called.\n");
   TCanvas::Draw(opt);
   this->FindObject("TFrame")->SetBit(TBox::kCannotMove);
}


bool GCanvas::HandleArrowKeyPress(Event_t *event,UInt_t *keysym) {

  TIter iter(gPad->GetListOfPrimitives());
  TH1 *hist = 0;
  while(TObject *obj = iter.Next()) {
     if( obj->InheritsFrom("TH1") &&
        !obj->InheritsFrom("TH2") &&  
        !obj->InheritsFrom("TH3") ) {  
        hist = (TH1*)obj; 
     }
  }
  if(!hist)
     return false;
  int first = hist->GetXaxis()->GetFirst();
  int last = hist->GetXaxis()->GetLast();
 
  int min = std::min(first,0);
  int max = std::max(last,hist->GetXaxis()->GetNbins()+1);


  //printf("first = %i  |  last = %i\n", first,last);
  //printf("min   = %i  |  max  = %i\n", min,max);

  int xdiff = last-first;
  int mdiff = max-min-2;
  //if(xdiff==mdiff)
  //   return;

  switch (*keysym) {
    case 0x1012: // left
     {
        if(mdiff>xdiff) {
          if(first==(min+1)) {
            //
          }
          else if((first-(xdiff/2))<min) {
            first = min+1;
            last  = min + (xdiff) + 1;
            //last  = first-min-1 + (xdiff/2); 
          } else {
            first = first-(xdiff/2); 
            last  = last -(xdiff/2);
          }
        }
        hist->GetXaxis()->SetRange(first,last);
        gPad->Modified();
        gPad->Update();
      }
      //printf("LEFT\n");
      break;
    case 0x1013: // up
      //printf("UP\n");
      break;
    case 0x1014: // right
     {
        //int xdiff = last-first;
        //int mdiff = max-min;
        if(mdiff>xdiff) {
          if(last== (max-1)) {
            // 
          }else if((last+(xdiff/2))>max) {
            first = max - 1 - (xdiff); 
            last  = max - 1;
          } else {
            last  = last +(xdiff/2); 
            first = first+(xdiff/2); 
          }
        }
        hist->GetXaxis()->SetRange(first,last);
        gPad->Modified();
        gPad->Update();
      }
      //printf("RIGHT\n");
      break;
    case 0x1015: // down
      //printf("DOWN\n");
      break;
    default:
      printf("keysym = %i\n",*keysym);
      break;
  }
  return true;

}


bool GCanvas::HandleKeyboardPress(Event_t *event,UInt_t *keysym) {

  //printf("keysym = %i\n",*keysym);
  TIter iter(gPad->GetListOfPrimitives());
  TH1 *hist = 0;
  bool edit = false;
  while(TObject *obj = iter.Next()) {
     if( obj->InheritsFrom("TH1") &&
        !obj->InheritsFrom("TH2") &&  
        !obj->InheritsFrom("TH3") ) {  
        hist = (TH1*)obj; 
     }
  }

  if(!hist)
     return false;


  switch(*keysym) {
    case kKey_b:
      edit = SetLinearBG();
      break;
    case kKey_E:
      GetContextMenu()->Action(hist->GetXaxis(),hist->GetXaxis()->Class()->GetMethodAny("SetRangeUser"));
      edit = true;
      break;
    case kKey_e:
      if(GetNMarkers()<2)
         break;
      if(fMarkers.at(fMarkers.size()-1)->localx < fMarkers.at(fMarkers.size()-2)->localx) 
        hist->GetXaxis()->SetRangeUser(fMarkers.at(fMarkers.size()-1)->localx,fMarkers.at(fMarkers.size()-2)->localx);
      else
        hist->GetXaxis()->SetRangeUser(fMarkers.at(fMarkers.size()-2)->localx,fMarkers.at(fMarkers.size()-1)->localx);
      edit = true;
      while(GetNMarkers())
         RemoveMarker();
      break;
    case kKey_g:
      edit = GausFit();
      break;
    case kKey_G:
      edit = GausBGFit();
      break;
    case kKey_m:
      SetMarkerMode(true);
      break;
    case kKey_M:
      SetMarkerMode(false);
    case kKey_n: 
      while(GetNMarkers())
         RemoveMarker();
      hist->GetListOfFunctions()->Delete();
      edit = true;
      break;  
    case kKey_o:
      hist->GetXaxis()->UnZoom();
      edit = true;    
      while(GetNMarkers())
         RemoveMarker();
      break;
    case kKey_S:
      if(fStatsDisplayed)
         fStatsDisplayed = false;
      else
         fStatsDisplayed = true;
      hist->SetStats(fStatsDisplayed);
      edit = true;
      break;
  };
  if(edit) {
    gPad->Modified();
    gPad->Update();
  }
  return true;
}


bool GCanvas::HandleMousePress(Int_t event,Int_t x,Int_t y) {
  //printf("Mouse clicked  %i   %i\n",x,y);
  if(!GetSelected())
    return false;
  //printf("GetSeleteced()->GetName() = %s\n",GetSelected()->GetName());
  //printf("GetSeleteced()->IsA() = %s\n",GetSelected()->IsA()->GetName());
  if(GetSelected()->InheritsFrom("TCanvas"))
     ((TCanvas*)GetSelected())->cd();


  //printf("GetSeleteced()->GetName() = %s\n",GetSelected()->GetName());
   
//    printf("GetSeleteced()->GetParenet()->GetName() = %s\n",GetSelected()->GetParent()->GetName());
//  printf("gPad                        = 0x%08x \n",gPad);
//  printf("this->GetClickSelectedPad() = 0x%08x \n",this->GetClickSelectedPad());


//  TPad *pad = Pick(x, y, 0);
//  if (!pad) return false;
//  pad->cd();
//  gROOT->SetSelectedPad(pad);


  TIter iter(gPad->GetListOfPrimitives());
  TH1 *hist = 0;
  bool edit = false;
  while(TObject *obj = iter.Next()) {
     if( obj->InheritsFrom("TH1") &&
        !obj->InheritsFrom("TH2") &&  
        !obj->InheritsFrom("TH3") ) {  
        hist = (TH1*)obj; 
     }
  }
  if(!hist)
     return false;

  bool used = false;

  if(!strcmp(gPad->GetSelected()->GetName(),"TFrame") && fMarkerMode) {
   ((TFrame*)gPad->GetSelected())->SetBit(TBox::kCannotMove);
    if(((GCanvas*)gPad->GetCanvas())->GetNMarkers()==4)
       ((GCanvas*)gPad->GetCanvas())->RemoveMarker();
    ((GCanvas*)gPad->GetCanvas())->AddMarker(x,y);
    //int px = gPad->AbsPixeltoX(x);
    //TLine *line = new TLine(px,GetUymin(),px,GetUymax());
    //line->Draw();
    used = true;
  }

  if(used) {
    gPad->Modified();
    gPad->Update();
  }
  return used;
}

bool GCanvas::SetLinearBG(GMarker *m1,GMarker *m2) {
  TIter iter(gPad->GetListOfPrimitives());
  TH1 *hist = 0;
  bool edit = false;
  while(TObject *obj = iter.Next()) {
     if( obj->InheritsFrom("TH1") &&
        !obj->InheritsFrom("TH2") &&  
        !obj->InheritsFrom("TH3") ) {  
        hist = (TH1*)obj; 
     }
  }
  if(!hist)
     return false;
  if(!m1 || !m2) {
    if(GetNMarkers()<2) {
       return false;
    } else { 
       m1 = fMarkers.at(fMarkers.size()-1);
       m2 = fMarkers.at(fMarkers.size()-2);
    }
  }
  TF1 *bg = hist->GetFunction("linbg");
  if(bg)
     bg->Delete();
  double x[2];
  double y[2];
  if(m1->localx < m2->localx) {
    x[0]=m1->localx; x[1]=m2->localx;
    y[0]=hist->GetBinContent(m1->x); y[1]=hist->GetBinContent(m2->x); 
  } else {
    x[1]=m1->localx; x[0]=m2->localx;
    y[1]=hist->GetBinContent(m1->x); y[2]=hist->GetBinContent(m2->x); 
  }
  printf("x[0] = %.02f   x[1] = %.02f\n",x[0],x[1]);
  bg = new TF1("linbg","pol1",x[0],x[1]);
  hist->Fit(bg,"QR+");
  bg->SetRange(gPad->GetUxmin(),gPad->GetUxmax());
  hist->GetListOfFunctions()->Add(bg);
  bg->Draw("same");
  //hist->GetListOfFunctions()->Add(bg);
  //bg->Draw("same");
  return true;
}

bool GCanvas::GausBGFit(GMarker *m1,GMarker *m2) {
  TIter iter(gPad->GetListOfPrimitives());
  TH1 *hist = 0;
  bool edit = false;
  while(TObject *obj = iter.Next()) {
     if( obj->InheritsFrom("TH1") &&
        !obj->InheritsFrom("TH2") &&  
        !obj->InheritsFrom("TH3") ) {  
        hist = (TH1*)obj; 
     }
  }
  if(!hist)
     return false;
  if(!m1 || !m2) {
    if(GetNMarkers()<2) {
       return false;
    } else { 
       m1 = fMarkers.at(fMarkers.size()-1);
       m2 = fMarkers.at(fMarkers.size()-2);
    }
  }
  
  TF1 *gausfit = hist->GetFunction("gausfit");
  if(gausfit)
     gausfit->Delete();
  int binx[2];
  double x[2];
  double y[2];
  if(m1->localx < m2->localx) {
    x[0]=m1->localx; x[1]=m2->localx;
    binx[0]=m1->x;   binx[1]=m2->x;
    y[0]=hist->GetBinContent(m1->x); y[1]=hist->GetBinContent(m2->x); 
  } else {
    x[1]=m1->localx; x[0]=m2->localx;
    binx[1]=m1->x;   binx[0]=m2->x;
    y[1]=hist->GetBinContent(m1->x); y[0]=hist->GetBinContent(m2->x); 
  }
  //printf("x[0] = %.02f   x[1] = %.02f\n",x[0],x[1]);
  gausfit = new TF1("gausfit","pol1(0)+gaus(2)",x[0],x[1]);
  TF1 *gfit = new TF1("gaus","gaus",x[0],x[1]);
  hist->Fit(gfit,"QR+");

  gausfit->SetParameters(y[0],0,gfit->GetParameter(0),gfit->GetParameter(1),gfit->GetParameter(2));
  
  gfit->Delete();
  hist->GetFunction("gaus")->Delete();

  hist->Fit(gausfit,"QR+");
  TF1 *bg = new TF1("bg","pol1",x[0],x[1]);
  bg->SetParameters(gausfit->GetParameter(0),gausfit->GetParameter(1));
  bg->Draw("same");
  hist->GetListOfFunctions()->Add(bg);
  
  double param[5];
  double error[5];
   
  gausfit->GetParameters(param);
  error[0] = gausfit->GetParError(0);
  error[1] = gausfit->GetParError(1);
  error[2] = gausfit->GetParError(2);
  error[3] = gausfit->GetParError(3);
  error[4] = gausfit->GetParError(4);
  
  printf("\nIntegral from % 4.01f to % 4.01f: %f\n",x[0],x[1],gausfit->Integral(x[0],x[1])/hist->GetBinWidth(1));
  printf("Centroid:  % 4.02f  +/- %.02f\n",param[3],error[3]);
  printf("FWHM:      % 4.02f  +/- %.02f\n",fabs(param[4]*2.35),error[4]*2.35);
  double integral = gausfit->Integral(x[0],x[1])/hist->GetBinWidth(1);
  double int_err  = integral*TMath::Sqrt(((error[2]/param[2])*(error[2]/param[2]))+
                                         ((error[4]/param[4])*(error[4]/param[4])));
  printf("Area:      % 4.02f  +/- %.02f\n",
         integral - (bg->Integral(x[0],x[1])/hist->GetBinWidth(1)),int_err);
  return true;
  
}

bool GCanvas::GausFit(GMarker *m1,GMarker *m2) {
  TIter iter(gPad->GetListOfPrimitives());
  TH1 *hist = 0;
  bool edit = false;
  while(TObject *obj = iter.Next()) {
     if( obj->InheritsFrom("TH1") &&
        !obj->InheritsFrom("TH2") &&  
        !obj->InheritsFrom("TH3") ) {  
        hist = (TH1*)obj; 
     }
  }
  if(!hist)
     return false;
  if(!m1 || !m2) {
    if(GetNMarkers()<2) {
       return false;
    } else { 
       m1 = fMarkers.at(fMarkers.size()-1);
       m2 = fMarkers.at(fMarkers.size()-2);
    }
  }
  
  TF1 *gausfit = hist->GetFunction("gausfit");
  if(gausfit)
     gausfit->Delete();
  int binx[2];
  double x[2];
  double y[2];
  if(m1->localx < m2->localx) {
    x[0]=m1->localx; x[1]=m2->localx;
    binx[0]=m1->x;   binx[1]=m2->x;
    y[0]=hist->GetBinContent(m1->x); y[1]=hist->GetBinContent(m2->x); 
  } else {
    x[1]=m1->localx; x[0]=m2->localx;
    binx[1]=m1->x;   binx[0]=m2->x;
    y[1]=hist->GetBinContent(m1->x); y[0]=hist->GetBinContent(m2->x); 
  }
  //printf("x[0] = %.02f   x[1] = %.02f\n",x[0],x[1]);
  gausfit = new TF1("gausfit","gaus",x[0],x[1]);
//  TF1 *gfit = new TF1("gaus","gaus",x[0],x[1]);
//  hist->Fit(gfit,"QR+");

  ///gausfit->SetParameters(y[0],0,gfit->GetParameter(0),gfit->GetParameter(1),gfit->GetParameter(2));
  
//  gfit->Delete();
  //hist->GetFunction("gaus")->Delete();

  hist->Fit(gausfit,"QR+");
  
  double param[3];
  double error[3];
   
  gausfit->GetParameters(param);
  error[0] = gausfit->GetParError(0);
  error[1] = gausfit->GetParError(1);
  error[2] = gausfit->GetParError(2);
  
  printf("\nIntegral from % 4.01f to % 4.01f: %f\n",x[0],x[1],gausfit->Integral(x[0],x[1])/hist->GetBinWidth(1));
  printf("Centroid:  % 4.02f  +/- %.02f\n",param[1],error[1]);
  printf("FWHM:      % 4.02f  +/- %.02f\n",param[2]*2.35,error[2]*2.35);
  double integral = gausfit->Integral(x[0],x[1])/hist->GetBinWidth(1);
  double int_err  = integral*TMath::Sqrt((error[0]/param[0])*(error[0]/param[0]) +
                                         ((error[2]/param[2])*(error[2]/param[2])));
  printf("Area:      % 4.02f  +/- %.02f\n",
         integral,int_err);
  return true;
  
}
