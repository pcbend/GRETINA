#ifndef GRUTCANVAS_H
#define GRUTCANVAS_H

#include "TCanvas.h"
#include "TRootCanvas.h"

class GCanvas : public TCanvas {
   public:
        GCanvas(Bool_t build = kTRUE);
        GCanvas(const char* name, const char* title = "", Int_t form = 1);
        GCanvas(const char* name, const char* title, Int_t ww, Int_t wh);
        GCanvas(const char* name, Int_t ww, Int_t wh, Int_t winid);
        GCanvas(const char* name, const char* title, Int_t wtopx, Int_t wtopy, Int_t ww, Int_t wh);
        virtual ~GCanvas();

        //void ProcessEvent(Int_t event,Int_t x,Int_t y,TObject *obj);
        //void CatchEvent(Int_t event,Int_t x,Int_t y,TObject *obj);

        void HandleInput(EEventType event,Int_t x,Int_t y);

        void Draw(Option_t *opt="");

   private:
      void GCanvasInit();

      void UpdateStatsInfo(int,int);

      ClassDef(GCanvas,1);

      static int lastx;
      static int lasty;


      void HandleKeyPress(int event,int x,int y,TObject *obj);

      Window_t fCanvasWindowID;
      TRootCanvas *fRootCanvas;

};

#endif
