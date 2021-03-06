#ifndef PWFRAGMENT_H 
#define PWFRAGMENT_H 


#include<vector>

#include <TObject.h>
#include <TCanvas.h>
#include <TH2F.h>
#include <TGEBEvent.h>

#include <PWSegment.h>
//class TGEBBankType1;

class PWFragment : public TObject {

  public:
    PWFragment();
    PWFragment(const PWFragment&);
    PWFragment(const TGEBEvent::TPWBank&);
    PWFragment(const TGEBEvent&);

    ~PWFragment();
    PWFragment& operator=(const PWFragment&);
    PWFragment& operator=(const TGEBEvent::TPWBank&);
    PWFragment& operator=(const TGEBEvent&);

    void Copy(const PWFragment&);
    void Copy(const TGEBEvent::TPWBank&);
    void Copy(const TGEBEvent&);  

    void Clear(Option_t *opt ="");
    void Print(Option_t *opt ="");
    void Draw(Option_t *opt="");
    TH2F MakeHitPattern(Option_t *opt="");
    void DrawSimpleHitPattern(Option_t *opt="");
      
    const inline Int_t   &GetNumberOfHits()           { return fNumberHits;  }

    inline PWSegment &GetSegment(unsigned int &i) 
                                      { if(i>=fSegments.size()) i=fSegments.size()-1; return fSegments.at(i); } 

    const inline PWSegment &Get(unsigned int &i) { return GetSegment(i); }  

    const inline Long_t GetTimeStamp() { return fTimeStamp; }

    const inline PWSegment &GetLargest()  { return fSegments.at(fLargestHit); }
    const inline Short_t    GetA()        { if(fLargestHit>=fSegments.size()) return 0; return fSegments.at(fLargestHit).A(); }
    const inline Short_t    GetB()        { if(fLargestHit>=fSegments.size()) return 0; return fSegments.at(fLargestHit).B(); }
    const inline Short_t    GetC()        { if(fLargestHit>=fSegments.size()) return 0; return fSegments.at(fLargestHit).C(); }
    const inline Short_t    GetLargestNumber() { return fLargestHit; }  

    const inline Int_t      GetAddress()  { if(fLargestHit>=fSegments.size()) return -1;return (((int)(fType<<24))+((int)(fSegments.at(fLargestHit).Pixel()))); }

    const inline Int_t      Address(const int &i) { int temp_add = (((int)(fType<<24))+((int)(fSegments.at(i).Pixel())));  return temp_add;   }
     


  private:
    Long_t  fTimeStamp;
    Int_t   fType;

    Short_t fLargestHit;

    Int_t   fNumberHits;
    std::vector<PWSegment> fSegments;

  ClassDef(PWFragment,1)
};



#endif 


