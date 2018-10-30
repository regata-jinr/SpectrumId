#ifndef SPECTRUM_H
#define SPECTRUM_H

 /* Copyright(c) 2018, REGATA Experiment at JINR, All rights reserved. *
  * See cxx source for full Copyright notice                           */
 
 
 /*!
  * \brief Class for works with spectrum.
  *
  * \author <a href="mailto:bdrum@jinr.ru">Boris Rumyantsev</a>
 */

 
 /*
  gSystem->AddIncludePath("-Iinc/")
  .L src/Spectrum.cxx+
  auto sp = new Spectrum()
  sp->Load("../peakdeco/peakdeco-84/data/check/7105423.txt")
  */
 
 
#include <Peak.h>
#include <Background.h>
#include <MetaData.h>

#include <string>
#include <fstream>
#include <vector>

 //TODO: add interface for load spectrum from DB
 //TODO: add try catch
 //TODO: add logs via 
 //TODO: add tests via googletest
 //TODO: add docs
class Spectrum {
  // members:
  private:
    MetaData metaData;
    const char defDelimiter;
    std::vector<int>* channelArr;
    std::vector<double>* energyArr;
    std::vector<int>* countsArr;
    int printLevel;
    std::string extArr[6];
    
  //methods:
  private:
    void LoadCnf(const std::string FileName, bool readMetaData);
    void LoadAscii(const std::string FileName, bool readMetaData);
    void Load(const std::string FileName, bool readMetaData);
    // Spectrum* Load(const std::string FileName); - from DB
    void ParseMetaDataCnf(const std::string); // NOTE here perhaps should be not a string, but something like byte string
    void ParseMetaDataAscii(const std::string);
    void ParseValue(const std::string);
   public:
    Spectrum();
    Spectrum(const char delimiter);
    Spectrum(const char* FileName, bool readMetaData=true) :defDelimiter('\t'), extArr{".CSV", ".csv", ".TXT", ".txt", ".CNF", ".cnf"}  {Spectrum(); Load(std::string(FileName), readMetaData);};
    void Load(const char* FileName, bool readMetaData=true) {return Load(std::string(FileName), readMetaData);};
    //void Save(Spectrum* s, const char* FileName); // NOTE why is it needed? It would be better if I can add opportunity to save some temporary states of spectrum;
    MetaData GetMetaData();
    int GetPeaksCount();
    Peak* GetPeaks();
    Background* GetBackground();
    std::vector<int>*    GetChannelArray() {return channelArr;};
    std::vector<double>* GetEnergyArray()  {return energyArr;};
    std::vector<int>*    GetCountsArray()  {return countsArr;};
    void SetChannelArray(std::vector<int>* channelArr)  {this->channelArr = channelArr;};
    void SetEnergyArray(std::vector<double>* energyArr) {this->energyArr = energyArr;};
    void SetCountsArray(std::vector<int>* countsArr)    {this->countsArr = countsArr;};
    void SetPrintLevel(int printLevel) {this->printLevel = printLevel;};
    int  GetPrintLevel() {return printLevel;};
    
};

#endif
