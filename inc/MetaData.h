#ifndef METADATA_H
#define METADATA_H

 /* Copyright(c) 2018, REGATA Experiment at JINR, All rights reserved. *
  * See cxx source for full Copyright notice                           */
 
 
 /*!
  * \brief Struct of MetaData filling from header of Spectrum.
  *
  * \author <a href"mailto:bdrum@jinr.ru">Boris Rumyantsev</a>
 */

#include <string>

//NOTE struct looks nice, but if I'll be use map, I can use some delimeter for headers (e.g. ':') and lvalue could be key, rval could be value. It means this program allows to use user own metadata! 
struct MetaData {
  std::string  sampleName;
  std::string  sampleId;
  std::string  sampleType;
  std::string  userName;
  std::string  sampleDesc;
  std::string  startTime;
  double       realTimeS;
  double       liveTimeS;
  unsigned int totalCounts;
  std::string  markerLeft;
  std::string  markerRight;
  unsigned int markerCounts;
  double       A0;
  double       A1;
  double       A2;
  double       A3;
  std::string  energyUnit;
  private:
    bool isInit = false;
  public:
    MetaData() {isInit = true;};
    bool empty() {
      return isInit;
    };
  
};
 
#endif
