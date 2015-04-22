
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

#include "GCanvas.h"
#include "GROOTGuiFactory.h"

#include <iostream>

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

   //if(gVirtualX->InheritsFrom("TGX11")) {
   //    printf("\tusing x11-like graphical interface.\n");
   //}
   //this->SetCrosshair(true);
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
   if(GetSelected() && !strcmp(GetSelected()->GetName(),"TFrame"))
     ((TFrame*)GetSelected())->SetBit(TBox::kCannotMove);
//   this->SetEditable(false);
//   printf(DRED);
//   printf("{GCanvas} HandleEvent:\n");
//   printf(DYELLOW "\tevent: \t0x%08x\n" DRED,event);
//   if(this->GetSelected())
//      printf("\tfselected found[0x%08x]\t %s\n",this->GetSelected(),this->GetSelected()->GetName());
//   printf(DBLUE);
   //Event_t ev;
   //int px,py;
   //printf("CheckEvent = %i\n",gVirtualX->CheckEvent(fCanvasWindowID,(EGEventType)event,ev));
   //printf("CheckEvent = %i\n",gVirtualX->CheckEvent(gVirtualX->GetCurrentWindow(),(EGEventType)event,ev));

   //gVirtualX->SelectWindow(this->GetCanvasID());
   //int myevent = gVirtualX->RequestLocator(1, 1, px, py);

   //printf("CheckEvent = %i\n",gVirtualX->CheckEvent(gVirtualX->GetDefaultRootWindow(),(EGEventType)event,ev));
   //printf("Events pending = %i\n",gVirtualX->EventsPending());

   //gVirtualX->NextEvent(ev);
   //printf("Event from request: %08x\n",&myevent);


//   printf(RESET_COLOR);
//  printf("I AM HERE!!!!!!!!!!\n");
  //If the below switch breaks. You need to upgrade your version of ROOT
  //Version 5.34.24 works.
  switch(event) {
//      case kArrowKeyPress:
      //case kArrowKeyRelease:
     // case kKeyPress: 
        //case kKeyRelease:
        //this->SetEditable(true);
     //   HandleKeyPress(event,x,y,this->GetSelected());
     //   break;
      case 0x00000001:
         HandleMousePress(event,x,y);
         break;
      default:
         //printf(RED"\t\tHANDLE DEFAULT!" RESET_COLOR "\n");
         TCanvas::HandleInput((EEventType)event,x,y);
         //printf("Window_t = 0x%08x\n",gVirtualX->GetCurrentWindow());
         break;
   };

   
   //TCanvas::ProcessedEvent(event,x,y,this->GetSelected());
   //this->SetEditable(fal);
   this->SetEditable(true);
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

void GCanvas::HandleKeyPress(int event,int x,int key,TObject *obj) {

   /*
   printf(DBLUE);
   //printf("\tevent  \t%i\n",this->GetEvent());
   std::cout << "\tevent  \t" << this->GetEvent() << "\n";
   printf("\tfsel:  \t%s\n",this->GetSelected()->GetName());
   printf("\tx:     \t%i\n",x);
   printf("\tkey:   \t%i\n",key);

   printf("\t\tGetEvent:    \t%i\n",this->GetEvent());
   printf("\t\tGetEventX:   \t%i\n",this->GetEventX());
   printf("\t\tGetEventY:   \t%i\n",this->GetEventY());


   printf(RESET_COLOR);
   switch(key) {
      case kKey_Up:
         printf("UP!\n");    
         if(obj)
            printf("\tobj = %s\n",obj->GetName());
         break;
      case kKey_Down:
         printf("DOWN!\n");         
         if(obj)
            printf("\tobj = %s\n",obj->GetName());
         break;
      case kKey_Right:
         printf("RIGHT!\n");         
         if(obj)
            printf("\tobj = %s\n",obj->GetName());
         break;
      case kKey_Left:
         printf("LEFT!\n");         
         if(obj)
            printf("\tobj = %s\n",obj->GetName());
         break;
   };
   */
}

void GCanvas::Draw(Option_t *opt) {
   printf("GCanvas Draw was called.\n");
   TCanvas::Draw(opt);
   this->FindObject("TFrame")->SetBit(TBox::kCannotMove);
}


void GCanvas::HandleArrowKeyPress(Event_t *event,UInt_t *keysym) {

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
     return;
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


}


void GCanvas::HandleKeyboardPress(Event_t *event,UInt_t *keysym) {

  printf("keysym = %i\n",*keysym);
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
     return;
  switch(*keysym) {
    case kKey_o:
      hist->GetXaxis()->UnZoom();
      edit = true;    
      break;
  };
  if(edit) {
    gPad->Modified();
    gPad->Update();
  }
  return;
}


void GCanvas::HandleMousePress(Int_t event,Int_t x,Int_t y) {
  printf("Mouse clicked\n");
  return;
}


