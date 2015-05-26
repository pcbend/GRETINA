#ifndef LaBrFRAGMENT_H 
#define LaBrFRAGMENT_H 


#include<vector>
#include<map>

#include <TObject.h>
#include <TCanvas.h>
#include <TH2F.h>
#include <TGEBEvent.h>

#include <LaBrSegment.h>
//class TGEBBankType1;

class LaBrFragment : public TObject {

  public:
    LaBrFragment();
    LaBrFragment(const LaBrFragment&);
    LaBrFragment(const TGEBEvent *event); //TGEBEvent::TLaBrBank&);
    //LaBrFragment(const TGEBEvent::TLaBrBank&);
    //LaBrFragment(const TGEBEvent&);

    ~LaBrFragment();
    LaBrFragment& operator=(const LaBrFragment&);
    //LaBrFragment& operator=(const TGEBEvent::TLaBrBank&);
    //LaBrFragment& operator=(const TGEBEvent&);

    void Copy(const LaBrFragment&);
    //void Copy(const TGEBEvent::TLaBrBank&);
    //void Copy(const TGEBEvent&);  

    void Clear(Option_t *opt ="");
    void Print(Option_t *opt ="") const;
      
    inline Int_t        Size() const        { return fSegments.size(); } //fNumberSegments;  }
    inline LaBrSegment &Get(const int &i)   { return fSegments.at(i); }  

    inline Long_t GetTimeStamp()        { return fTimeStamp; }

    LaBrSegment *FindSegment(const int &id);

    inline Int_t      &GetTDCRef()                  { return fTdcReference; }
    inline void        SetTDCRef(const int &tdc_ref) { fTdcReference = tdc_ref; }


    const inline Int_t GetAddress(const int &i) { return ((fType<<24)+(fSegments.at(i).ID())); }

  private:
    Int_t   fType;
    Long_t  fTimeStamp;
    Int_t   fNumberSegments;
    Int_t   fTdcReference;
    std::vector<LaBrSegment> fSegments;
    std::map<int,int> fIdVecMap;    

  ClassDef(LaBrFragment,1)
};



#endif 


