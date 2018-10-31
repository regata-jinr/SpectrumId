/**************************************************************************
 *                                                                        *
 *                                                                        *
 * Copyright(c) 2018, REGATA Experiment at JINR, All rights reserved.     *
 *                                                                        *
 *                                                                        *
 **************************************************************************/

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "Spectrum.h"
#include "TSpectrum.h"
#include "TError.h"
#include "MetaData.h"

//TODO: add destructor;
//TODO: metaData.empty() is not defined;
//TODO: Should we let to users specify delimiter?
//NOTE: LoadCnf could be skip, because now we can focus on algorithms.

Spectrum::Spectrum()
  :defDelimiter('\t'),
   extArr{".CSV", ".csv", ".TXT", ".txt", ".CNF", ".cnf"}
{
    //defDelimiter('\t');
    channelArr   = new std::vector<int>();
    channelArr->reserve(8192);
    energyArr    = new std::vector<double>();
    energyArr->reserve(8192);
    countsArr    = new std::vector<int>();
    countsArr->reserve(8192);
    SetPrintLevel(0);
}

Spectrum::Spectrum(const char delimiter) 
  :defDelimiter(delimiter),
   extArr{".CSV", ".csv", ".TXT", ".txt", ".CNF", ".cnf"}
{
    channelArr   = new std::vector<int>();
    channelArr->reserve(8192);
    energyArr    = new std::vector<double>();
    energyArr->reserve(8192);
    countsArr    = new std::vector<int>();
    countsArr->reserve(8192);
    SetPrintLevel(0);
}

MetaData Spectrum::GetMetaData() {
  //if (metaData.empty()) ::Warning("Spectrum::GetMetaData()", "metaData is empty!"); 
  return metaData;
}

void Spectrum::ParseMetaDataCnf(const std::string line) {
  return;
  uint32_t idh=0;
  size_t oh = 0x70;
  int i = 0;
  char* data = nullptr;
  while (idh!=0x00000000) {
    oh +=i*0x30;
    //idh = (uint32_t)(data + oh);
    i++;
  }
  
  
}

void Spectrum::ParseMetaDataAscii(const std::string line) {
}

void Spectrum::Load(const std::string FileName, bool readMetaData) {
  const std::string ext = FileName.substr(FileName.find_last_of('.'));
  if (std::find(std::begin(extArr), std::end(extArr), ext) == std::end(extArr))
    {::Error("Spectrum::Load", "Incorrect extension of input file!"); return;}
  
  if (ext.compare(extArr[4]) == 0 || ext.compare(extArr[5]) == 0)
    return Spectrum::LoadCnf(FileName, readMetaData);
  else
    return Spectrum::LoadAscii(FileName, readMetaData);
}

void Spectrum::LoadCnf(const std::string FileName, bool readMetaData) {
  ::Info("Spectrum::LoadCnf", "Function don't implement yet. Please, use cnf2txt.");
  return;
  std::ifstream iFile(FileName.c_str(), std::ios::binary | std::ios::ate);
  if (!iFile.is_open()) {::Error("Spectrum::LoadCnf", "File can't be open!"); return;}
  std::streamsize size = iFile.tellg();
  iFile.seekg(0, std::ios::beg);
  //Spectrum::ParseMetaDataCnf(iFile);
  std::vector<char> buffer(size);
  if (iFile.read(buffer.data(), size)) {
    /* worked! */
  }
  iFile.close();
}

void Spectrum::LoadAscii(const std::string FileName, bool readMetaData) {
  std::ifstream iFile(FileName.c_str());
  std::string line="#";
  if (!iFile.is_open()) {::Error("Spectrum::LoadAscii", "File can't be open!"); return;}
  
  if (readMetaData)
    while(std::getline(iFile, line) && line[0] == '#') Spectrum::ParseMetaDataAscii(line);
  iFile.tellg();
  // TODO: check delimiter!
  // if (line.find(defDelimiter) == std::string::npos) {::Error("Spectrum::LoadAscii","Default delimiter %c not found in first non header line.", defDelimiter);return;}
  Spectrum::ParseValue(line);
  while(std::getline(iFile, line)) Spectrum::ParseValue(line);
  iFile.close();
}

void Spectrum::ParseValue(const std::string line) {
  std::istringstream iss(line);
  std::string token;
  //TODO: check that channel,energy and counts exists!
  std::getline(iss, token, defDelimiter); 
  channelArr->push_back(std::stoi(token));
  std::getline(iss, token, defDelimiter);
  energyArr->push_back(std::stod(token));
  std::getline(iss, token, defDelimiter); 
  countsArr->push_back(std::stoi(token));
}

