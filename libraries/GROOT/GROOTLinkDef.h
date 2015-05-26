
#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclasses;

#pragma link C++ class GROOTGuiFactory+;
#pragma link C++ class GRootCanvas+;
#pragma link C++ class GRootBrowser+;

#pragma link C++ class GPadObj+;
#pragma link C++ class GMemObj+;
#pragma link C++ class GRootObjectManager+;

#pragma link C++ function ProjectionX(TH2*,int,int);
#pragma link C++ function ProjectionY(TH2*,int,int);
#pragma link C++ function ProjectionX(TH2*,double,double);
#pragma link C++ function ProjectionY(TH2*,double,double);

#pragma link C++ function PeakSearch(TH1*,double);
#pragma link C++ function Prompt();
#pragma link C++ function SaveAll(const char*,Option_t*);

#pragma link C++ function Help;
#pragma link C++ function Commands;


#pragma link C++ class GH1D+;
#pragma link C++ class GMarker+;
#pragma link C++ class GCanvas+;
#pragma link C++ class GDirectory+;

#pragma link C++ class GFile+;
#pragma link C++ class GTree+;
#pragma link C++ class GChain+;



#endif

