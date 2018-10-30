#ifndef SPECTRUM_H
#define SPECTRUM_H

 /* Copyright(c) 2018, REGATA Experiment at JINR, All rights reserved. *
  * See cxx source for full Copyright notice                           */
 
 
 /*!
  * \brief Class for works with spectrum.
  *
  * \author <a href="mailto:bdrum@jinr.ru">Boris Rumyantsev</a>
 */

#include <Peak.h>
#include <Background.h>
#include <MetaData.h>

#include <string>
#include <fstream>
#include <valarray>

 //TODO: add interface for load spectrum from DB
 //TODO: add try catch
 //TODO: add logs via 
 //TODO: add tests via googletest
 //TODO: add docs
class Spectrum {
  // members:
  private:
    std::string extArr[6] = {".CSV", ".csv", ".TXT", ".txt", ".CNF", ".cnf"};
    MetaData metaData;
    const char defDelimiter = '\t';
    std::valarray<int>* channelArr;
    std::valarray<double>* energyArr;
    std::valarray<int>* countsArr;
    int printLevel;
    
  //methods:
  private:
    void LoadCnf(const std::string FileName, bool readMetaData);
    void LoadAscii(const std::string FileName, bool readMetaData);
    void Load(const std::string FileName, bool readMetaData);
    // Spectrum* Load(const std::string FileName); - from DB
    void ParseMetaDataCnf(const std::string&); // NOTE here perhaps should be not a string, but something like byte string
    void ParseMetaDataAscii(const std::string&);
    void ParseValue(const std::string);
   public:
    Spectrum();
    Spectrum(const char delimiter);
    Spectrum(const char* FileName, bool readMetaData=true) {Spectrum(); Load(std::string(FileName), readMetaData);};
    void Load(const char* FileName, bool readMetaData=true) {return Load(std::string(FileName), readMetaData);};
    //void Save(Spectrum* s, const char* FileName); // NOTE why is it needed? It would be better if I can add opportunity to save some temporary states of spectrum;
    MetaData GetMetaData();
    int GetPeaksCount();
    Peak* GetPeaks();
    Background* GetBackground();
    std::valarray<int>*    GetChannelArray() {return channelArr;};
    std::valarray<double>* GetEnergyArray()  {return energyArr;};
    std::valarray<int>*    GetCountsArray()  {return countsArr;};
    void SetChannelArray(std::valarray<int>* channelArr)  {this->channelArr = channelArr;};
    void SetEnergyArray(std::valarray<double>* energyArr) {this->energyArr = energyArr;};
    void SetCountsArray(std::valarray<int>* countsArr)    {this->countsArr = countsArr;};
    void SetPrintLevel(int printLevel) {this->printLevel = printLevel;};
    int  GetPrintLevel() {return printLevel;};
    
};

#endif
