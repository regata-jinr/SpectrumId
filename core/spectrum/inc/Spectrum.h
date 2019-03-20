#ifndef SPECTRUM_H
#define SPECTRUM_H

 /* Copyright(c) 2018-2019, REGATA Experiment at JINR, All rights reserved. *
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
 
 
#include "Peak.h"
#include "Background.h"
#include "MetaData.h"

#include <string>
#include <string_view>
#include <fstream>
#include <vector>
#include <iterator>

 //TODO: add interface for load spectrum from DB
 //TODO: add try catch
 //TODO: add logs via glog
 //TODO: add tests via googletest
 //TODO: add documentation
 
 namespace SpectrumId {
   
  enum FileExtesions {txt, cnf, csv}; 
  enum Verbosity {Info, Debug, Warning, Error}; 
   
  class Spectrum {
    private:
      MetaData metaData;
      char defDelimiter;
      std::vector<double> channelArr;
      std::vector<double> energyArr;
      std::vector<double> countsArr;
      //todo add enumeration or use root kWarning, kInfo, etc.
      int printLevel;
      void LoadCnf(const std::string FileName, bool readMetaData);
      void LoadAscii(const std::string FileName, bool readMetaData);
      void Load(const std::string FileName, bool readMetaData);
      // Spectrum* Load(const std::string FileName); - from DB
      void ParseMetaDataCnf(const std::string); // NOTE here perhaps the argument should be not a string, but something like byte string
      void ParseMetaDataAscii(const std::string);
      void ParseValue(const std::string);
    public:
      Spectrum(const char* FileName, bool readMetaData=true);
      //void Save(Spectrum* s, const char* FileName); // NOTE why is it needed? It would be better if I can add opportunity to save some temporary states of spectrum;
      MetaData GetMetaData();
      int GetPeaksCount();
      // Peak* GetPeaks();
      // Background* GetBackground();
      std::vector<double>    GetChannelArray() {return channelArr;};
      std::vector<double> GetEnergyArray()  {return energyArr;};
      std::vector<double>    GetCountsArray()  {return countsArr;};
      void SetPrintLevel(int printLevel) {this->printLevel = printLevel;};
      int  GetPrintLevel() {return printLevel;};
      void SetDelimiter(const char delimiter) {this->defDelimiter = delimiter;};
      char GetDelimiter() {return this->defDelimiter;};
      void Smooth();
      ~Spectrum(){};
    };
}
#endif
