/***************************************************************************
 *                                                                         *
 *                                                                         *
 * Copyright(c) 2018-2019, REGATA Experiment at JINR, All rights reserved. *
 *                                                                         *
 *                                                                         *
 ***************************************************************************/

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "Spectrum.h"
#include "TSpectrum.h"
#include "MetaData.h"
#include "Window.h"
#include "Utilities.h"


//TODO: add destructors; USE ONLY SMART POINTERS
//TODO: metaData.empty() is not defined;
//TODO: Should we let to users specify delimiter?
//NOTE: LoadCnf could be skip, because now we can focus on algorithms.


/**
* @brief ...
* 
* @param FileName p_FileName:...
* @param readMetaData p_readMetaData:...
*/
void SpectrumId::Spectrum::LoadCnf(const std::string FileName, bool readMetaData) {
  //::Info("Spectrum::LoadCnf", "Function don't implement yet. Please, use cnf2txt.");
  return;
  std::ifstream iFile(FileName.c_str(), std::ios::binary | std::ios::ate);
//   if (!iFile.is_open()) {::Error("Spectrum::LoadCnf", "File can't be open!"); return;}
  std::streamsize size = iFile.tellg();
  iFile.seekg(0, std::ios::beg);
  //Spectrum::ParseMetaDataCnf(iFile);
  std::vector<char> buffer(size);
  if (iFile.read(buffer.data(), size)) {
    /* worked! */
  }
  iFile.close();
}

/**
* @brief ...
* 
* @param FileName p_FileName:...
* @param readMetaData p_readMetaData:...
*/
// void SpectrumId::Spectrum::LoadAscii(const std::string FileName, bool readMetaData) {
//   std::ifstream iFile(FileName.c_str());
//   std::string line="#";
// //   if (!iFile.is_open()) {::Error("Spectrum::LoadAscii", "File can't be open!"); return;}
//   
//   if (readMetaData)
//     while(std::getline(iFile, line) && line[0] == '#') Spectrum::ParseMetaDataAscii(line);
//   iFile.tellg();
//   // TODO: check delimiter!
//   // if (line.find(defDelimiter) == std::string::npos) {::Error("Spectrum::LoadAscii","Default delimiter %c not found in first non header line.", defDelimiter);return;}
//   Spectrum::ParseValue(line);
//   while(std::getline(iFile, line)) Spectrum::ParseValue(line);
//   iFile.close();
// }

/**
* @brief ...
* 
* @param line p_line:...
*/
void SpectrumId::Spectrum::ParseValue(const std::string line) {
  std::istringstream iss(line);
  std::string token;
  //TODO: check that channel,energy and counts exists!
  //TODO: how can I change such construction to just iFile >> channel >> energy >> counts; ?
  std::getline(iss, token, defDelimiter); 
  channelArr.push_back(std::stoi(token));
  std::getline(iss, token, defDelimiter);
  energyArr.push_back(std::stod(token));
  std::getline(iss, token, defDelimiter); 
  countsArr.push_back(std::stoi(token));
}

void SpectrumId::Spectrum::Smooth() {
  
}
