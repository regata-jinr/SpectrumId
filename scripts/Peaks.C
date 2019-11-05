#include "TMath.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Spectrum.h"
#include "TSpectrum.h"
#include "TSystemDirectory.h"
#include <chrono>
#include "TList.h"

Double_t* SMNP(Double_t* arr, Int_t size) {
  auto sArr = new double[size];
  for (auto i=0;i<size;++i) sArr[i]=TMath::Log(TMath::Log(TMath::Sqrt(arr[i]+1)+1)+1);
  return sArr;
}


//"/home/bdrum/GoogleDrive/Job/flnp/srec/spectra/7105423.txt"
void SearchPeaksForPoster() {
  
//   auto sp = new Spectrum("../peakdeco/peakdeco-84/data/check/7105423.txt");
  
  std::vector<int> cnts = {1,2,5,10,25,50,100};

  TString pathWithSpectra = "/home/bdrum/GoogleDrive/Job/flnp/srec/data/perfomanceTestTXT/";
  TString path2Save = "/home/bdrum/GoogleDrive/Job/flnp/srec/data/rootReps/";
  TString fullName;
  std::cout << "counts" << "\t" << "time,s" << std::endl;

  for (auto j:cnts) {
    auto startTime = std::chrono::high_resolution_clock::now();
    TSystemDirectory dir("perf", pathWithSpectra.Data());
    auto fileList = dir.GetListOfFiles();
    for (auto i=0; i < fileList->GetEntries(); ++i) {
      fullName = pathWithSpectra + TString(fileList->At(i)->GetName());
      if (i == j) break;
      if (!fullName.EndsWith("txt")) continue;
      auto sp = new Spectrum(fullName.Data());
      auto tsp = new TSpectrum(300, 1);
      auto countsI = sp->GetCountsArray();
      std::vector<double> countsV(countsI.begin(), countsI.end());
      auto energyV = sp->GetEnergyArray();
      Double_t* e = &energyV[0];
      Double_t* c = &countsV[0]; //SMNP((double *)countsV.data(), countsV.size());
      auto destC = new double[countsV.size()];
      int nFound = tsp->SearchHighRes(c,destC,countsV.size(),4, 0.05, kTRUE, 20, kTRUE, 3);
      std::ofstream out(path2Save+TString("rep_")+TString(fileList->At(i)->GetName()));
      out << "PeakNum\tPeakPosition\n";
      for (auto k=0; k < tsp->GetNPeaks(); ++k)
          out << k << "\t" << tsp->GetPositionX()[k] << "\n";
      out.close();
    }
    auto finishTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finishTime - startTime;
    std::cout << j << "\t" << elapsed.count() << std::endl;
  }
  
}

  
/**
 * References:
 * [1] M. Morháč, J. Kliman, V. Matoušek, M. Veselský, I. Turzo.: Background elimination methods for multidimensional gamma-ray spectra. NIM, A401 (1997) 113-132.
 */
