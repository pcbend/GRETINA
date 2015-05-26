#ifndef TCHANNEL_H
#define TCHANNEL_H

/*
 * Author:  P.C. Bender, <pcbend@gmail.com>
 * 
 * Please indicate changes with your initials.
 * 
 *
 */


 /////////////////////////////////////////////////////////////////////////
 //                                                                     //
 // TChannel                                                            //
 //                                                                     //
 // The TChannel is designed to hold all non-essential                  // 
 // information of an arbitraty Fragment (name, energy coeff, etc..)    //
 // that would otherwise clog up the Tree.  The TChannel class          //
 // contains a static map to every channel make retrieval fairly        //
 // easy.  The TChannel class also contains the ability to              //
 // read and write a custom calibration file to set or                  //
 // save the TChannel information.                                      //
 //                                                                     //
 /////////////////////////////////////////////////////////////////////////


#include <string>
#include <cmath>
#include <utility>
#include <map>

#include "TNamed.h"
#include "TRandom.h"
#include "TFile.h"
//#include "TList.h"
//#include "TTree.h"

#include "Globals.h"

class TChannel : public TNamed	{

  public:
    static TChannel * const GetChannel(unsigned int temp_address); 
    static TChannel * const FindChannelByName(const char *name);

    TChannel();
    TChannel(const char*);
    TChannel(TChannel*);

    virtual ~TChannel(); 

    static int  GetNumberOfChannels() { return fChannelMap->size(); }
    static void AddChannel(TChannel*,Option_t *opt="");
    static int  UpdateChannel(TChannel*,Option_t *opt="");


    static std::map<unsigned int,TChannel*> *GetChannelMap() { return fChannelMap; }
    static void DeleteAllChannels();

    static bool Compare(const TChannel&,const TChannel&);

    static TChannel *GetDefaultChannel();

  private:
    unsigned int	   address;                                 //The address of the channel
    std::string      channelname;                             //The name of the channel (MNEMONIC)
    std::string      type_name;
    int 	            number;

    std::vector<double> ENGCoefficients;  //Energy calibration coeffs (low to high order)
    double ENGChi2;                       //Chi2 of the energy calibration
    std::vector<double> EFFCoefficients;  //Efficiency calibration coeffs (low to high order)
    double EFFChi2;                       //Chi2 of Efficiency calibration
    std::vector<double> CFDCoefficients;  //CFD calibration coeffs (low to high order)
    double CFDChi2;                       //Chi2 of the CFD calibration
    std::vector<double> LEDCoefficients;  //LED calibration coeffs (low to high order)
    double LEDChi2;                       //Chi2 of LED calibration
    std::vector<double> TIMECoefficients; //Time calibration coeffs (low to high order)
    double TIMEChi2;                      //Chi2 of the Time calibration

    static std::map<unsigned int,TChannel*> *fChannelMap; //A map to all of the channels based on address
    static void UpdateChannelMap();
	 void OverWriteChannel(TChannel*);
	 void AppendChannel(TChannel*);

    void SetENGCoefficients(std::vector<double> tmp) { ENGCoefficients = tmp; }
    void SetEFFCoefficients(std::vector<double> tmp) { EFFCoefficients = tmp;}
    void SetCFDCoefficients(std::vector<double> tmp) { CFDCoefficients = tmp; }
    void SetLEDCoefficients(std::vector<double> tmp) { LEDCoefficients = tmp; }
    void SetTIMECoefficients(std::vector<double> tmp){ TIMECoefficients = tmp; }
	
	 static void trim(std::string *, const std::string & trimChars = " \f\n\r\t\v");


  public:
    void SetAddress(unsigned int tmpadd);
    inline void SetChannelName(const char *tmpname)  {channelname.assign(tmpname);} 
    inline void SetNumber(int tmpnum)	              {number = tmpnum;   }
    inline void SetTypeName(std::string tmpstr)      {type_name = tmpstr;}
   

    int	GetNumber()		           { return number;  }
    unsigned int	GetAddress()     { return address; }
    const char *GetChannelName()   { return channelname.c_str(); }
    const char *GetTypeName()      { return type_name.c_str(); }

    double GetENGChi2()  { return ENGChi2; }
    double GetEFFChi2()  { return EFFChi2;} 
    double GetCFDChi2()  { return CFDChi2; }
    double GetLEDChi2()  { return LEDChi2; }
    double GetTIMEChi2() { return TIMEChi2; }

    std::vector<double> GetENGCoeff()  { return ENGCoefficients;}
    std::vector<double> GetEFFCoeff()  { return EFFCoefficients;}
    std::vector<double> GetCFDCoeff()  { return CFDCoefficients;}
    std::vector<double> GetLEDCoeff()  { return LEDCoefficients;}
    std::vector<double> GetTIMECoeff() { return TIMECoefficients;}

    inline void AddENGCoefficient(double temp)  { ENGCoefficients.push_back(temp); }
    inline void AddEFFCoefficient(double temp)  { EFFCoefficients.push_back(temp);}
    inline void AddCFDCoefficient(double temp)  { CFDCoefficients.push_back(temp); }
    inline void AddLEDCoefficient(double temp)  { LEDCoefficients.push_back(temp); }
    inline void AddTIMECoefficient(double temp) { TIMECoefficients.push_back(temp);}


    inline void SetENGChi2(double temp)  { ENGChi2 = temp; }
    inline void SetEFFChi2(double temp)  { EFFChi2 = temp; } 
    inline void SetCFDChi2(double temp)  { CFDChi2 = temp; }
    inline void SetLEDChi2(double temp)  { LEDChi2 = temp; }
    inline void SetTIMEChi2(double temp) { TIMEChi2 = temp; }

    //void CalibrateFragment(TFragment*);

    double CalibrateENG(double);
    double CalibrateENG(int);
    
    double CalibrateCFD(double);
    double CalibrateCFD(int);
    
    double CalibrateLED(double);
    double CalibrateLED(int); 

    double CalibrateTIME(double);
    double CalibrateTIME(int);

    double CalibrateEFF(double);

    void DestroyCalibrations();

    void DestroyENGCal();
    void DestroyCFDCal();
    void DestroyLEDCal();
    void DestroyTIMECal();
    void DestroyEFFCal();

    static Int_t ReadCalFile(const char *filename = "");
    static Int_t ParseInputData(const char *inputdata = "",Option_t *opt = "");
    static void WriteCalFile(std::string outfilename = "");
    static void WriteCalBuffer(Option_t *opt ="");

    virtual void Print(Option_t *opt = "") const;
    virtual void Clear(Option_t *opt = "");
    //static  void PrintAll(Option_t *opt = "");      
    std::string PrintToString(Option_t *opt="");

    static int WriteToRoot(TFile *fileptr = 0);

  
  private:
     // the follow is to make the custum streamer 
     // stuff play nice.  pcb.
     static std::string fFileName;
     static std::string fFileData;
     static void InitChannelInput();
     static void SaveToSelf(const char*);

    ClassDef(TChannel,5) //Contains the Digitizer Information
};
#endif
