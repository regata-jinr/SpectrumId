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
  auto sp = new Spectrum("../peakdeco/peakdeco-84/data/check/7105423.txt")
  */
 
 
#include <Peak.h>
#include <Background.h>
#include <MetaData.h>

#include <string>
#include <fstream>
#include <vector>
#include <iterator>

 //TODO: add interface for load spectrum from DB
 //TODO: add try catch
 //TODO: add logs via 
 //TODO: add tests via googletest
 //TODO: add docs
class Spectrum {
  // fields:
  private:
    MetaData metaData;
    const char defDelimiter;
    std::vector<int> channelArr;
    std::vector<double> energyArr;
    std::vector<int> countsArr;
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
    void InitFields();
   public:
    Spectrum();
    Spectrum(const char delimiter);
    Spectrum(const char* FileName, bool readMetaData=true);
    //void Save(Spectrum* s, const char* FileName); // NOTE why is it needed? It would be better if I can add opportunity to save some temporary states of spectrum;
    MetaData GetMetaData();
    int GetPeaksCount();
    // Peak* GetPeaks();
    // Background* GetBackground();
    std::vector<int>    GetChannelArray() {return channelArr;};
    std::vector<double> GetEnergyArray()  {return energyArr;};
    std::vector<int>    GetCountsArray()  {return countsArr;};
    //TODO: should I use size_t here?
    //TODO: Change raw pointer to unique_pointer
    void SetChannelArray(int* channelArr, size_t n)  {this->channelArr.assign(channelArr, channelArr+n);};
    void SetEnergyArray(double* energyArr, size_t n) {this->energyArr.assign(energyArr, energyArr+n);};
    void SetCountsArray(int* countsArr, size_t n)    {this->countsArr.assign(countsArr, countsArr+n);};
    void SetPrintLevel(int printLevel) {this->printLevel = printLevel;};
    int  GetPrintLevel() {return printLevel;};
    
};

#endif
