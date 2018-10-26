#ifndef DATAWORKER_H
#define DATAWORKER_H

 /* Copyright(c) 2018, REGATA Experiment at JINR, All rights reserved. *
  * See cxx source for full Copyright notice                           */
 
 
 /*!
  * \brief Class for work with the data:
  * Reading and writing files.
  * Parsing to arrays etc...
  *
  * \author <a href="mailto:bdrum@jinr.ru">Boris Rumyantsev</a>
 */

#include <cstring>
#include <map>
#include <vector>
#include <valarray>

// TODO: now we use side converter from cnf to txt. let's try to use canberra libraries:
// #import "DataAccess.tlb" no_namespace raw_interfaces_only

 
class DataWorker {
  private:
  static std::map<std::string, std::valarray<double>> Cnf2Map(const char* FileName);
  static std::map<std::string, std::valarray<double>> Ascii2Map(const char* FileName);
  public:
    static std::map<std::string, std::valarray<double>> File2Map(const char* FileName, bool loadMetaData=false);
    static std::map<std::string, std::string> GetMetadata(const char* FileName);
    static void Map2CSV(static std::map<std::string, std::valarray<double>>, const char* FileName);

//     ClassDef(DataWorker, 1);
};

#endif
