#include "TCanvas.h"
#include "TMath.h"
#include "TH1.h"
#include <iostream>
#include <sstream>
#include "Spectrum.h"
#include "TSpectrum.h"
#include "TLegend.h"
#include "TPolyMarker.h"


void TryIt() {
  auto *his = new TH1D("his", "his",1000,-100,100);
  his->FillRandom("gaus",100000);
  his->Draw();
}


/*
 * [extractData description]
 * @param fileName [name of data file]
 * @param start    [start getline]
 * @param finish   [end getline]
 */
std::map<std::string, std::vector<double>> extractData(std::string fileName, Int_t start=0, Int_t finish=0) {
  std::fstream fs(fileName);
  std::string st;
  size_t pos = 0;
  std::string token;
  std::map<std::string, std::vector<double>> data;
  std::vector<std::string> columns;
  columns.push_back("n");
  columns.push_back("energy(keV)");
  columns.push_back("counts");
  columns.push_back("rate(1/s)");
  int num, row = 0;

  while (getline(fs,st)) {
    if (st[0] == '#') continue;
    num=0;
    while ((pos = st.find("\t")) != std::string::npos && row >=500 && row <=2000) {
      token = st.substr(0, pos);
      data[columns[num]].push_back(std::stod(token));
      st.erase(0, pos + 1);
      num++;
    }
    row++;
  }
return data;
}

/**
 * [FillHist description]
 * @param  fileName  [txt file with data]
 * @param  showStats [if true statistical info will be shown. including: size of input data, maxVal, minVal, meanVal, q1 (x_0.25), q2, q3]
 * @return           [filled histogram]
 */
TH1D *FillHist(const char *fileName, Bool_t showStats=kFALSE, Bool_t LLS=kFALSE) {
  auto data = extractData(fileName);
  auto his = new TH1D("h1", "Searching of peaks", data["energy(keV)"].size() - 1, &data["energy(keV)"][0]);

  if (LLS) {
    std::vector<Double_t > v;
    for (auto &d: data["counts"]) v.push_back(TMath::Log(TMath::Log(TMath::Sqrt(d+1)+1)+1));
    data["counts"] = v;
  }

  for (uint i = 0; i < data["counts"].size(); ++i) his->SetBinContent(i + 1, data["counts"][i]);

  if (!showStats) return his;
  else {
    auto sortedCounts = data["counts"];
    std::sort(sortedCounts.begin(), sortedCounts.end());
    std::cout << "size = " << sortedCounts.size() << std::endl;
    std::cout << "max = " << *std::max_element(sortedCounts.begin(), sortedCounts.end()) << std::endl;
    std::cout << "min = " << *std::min_element(sortedCounts.begin(), sortedCounts.end()) << std::endl;
    std::cout << "mean = " << TMath::Mean(sortedCounts.begin(), sortedCounts.end()) << std::endl;
    Double_t q[3] = {0.25, 0.5, 0.75};
    Double_t xq[3];
    TMath::Quantiles(sortedCounts.size(), 3, &sortedCounts[0], &xq[0], &q[0]);
    for (auto i = 0; i < 3; ++i)
      std::cout << "x_" << q[i] << "=" << xq[i] << std::endl;
    return his;
  }
}

/**
 * [FitBackground description]
 */
void FitBackground(const char *fileName) {
  auto h = FillHist(fileName, kTRUE, kTRUE);
  TSpectrum *s = new TSpectrum();
  Int_t nbins, bin;
  nbins = h->GetSize();
  Double_t source[nbins];
  for (auto i = 0; i < nbins; i++) source[i]=h->GetBinContent(i + 1);

  TCanvas *c1 = new TCanvas("c1","c1",10,10,1000,900);
  c1->cd();

  h->Draw();
  s->Background(h, 50,"backorder4 same compton");

  //s->Background(h, 50,"backsmoothing15 compton backorder8 same");
}

TPolyMarker* SearchPeaksPD(const char *fileName);
TPolyMarker* SearchPeaksGenie(const char *fileName);

//"/home/bdrum/GoogleDrive/Job/flnp/srec/spectra/7105423.txt"
void SearchPeaks(const char *fileName, Double_t sig=1, Option_t *opt="", Double_t treshld=0.05, Int_t iterations=3, Int_t averW=5) {
  auto h = FillHist(fileName, kTRUE, kTRUE);
  h->SetStats(kFALSE);
  h->SetMarkerStyle(8);
  h->SetMarkerColor(1);
  h->SetMarkerSize(0.4);
  TSpectrum *s = new TSpectrum();
  s->SetAverageWindow(averW);
  s->SetDeconIterations(iterations);
  s->Search(h,sig, opt, treshld);

  TPolyMarker* pm = (TPolyMarker*)h->GetListOfFunctions()->FindObject("TPolyMarker");
  if (pm) {
    h->GetListOfFunctions()->Remove(pm);
    delete pm;
  }
  pm = new TPolyMarker(s->GetNPeaks(), s->GetPositionX(), s->GetPositionY());
  h->GetListOfFunctions()->Add(pm);
  pm->SetMarkerStyle(23);
  pm->SetMarkerColor(kRed);
  pm->SetMarkerSize(1.3);
  
  auto pmPD = SearchPeaksPD("/home/bdrum/GoogleDrive/Job/flnp/srec/peakdeco/peakdeco-84/data/check/peaks.dat");
  auto pmG = SearchPeaksGenie("/home/bdrum/GoogleDrive/Job/flnp/srec/peakdeco/peakdeco-84/data/check/7105423_FOUNDED.txt");
  h->GetListOfFunctions()->Add(pmPD);
  h->GetListOfFunctions()->Add(pmG);

  h->Draw("p");

  std::cout << "The identified peaks by root are:" << std::endl;
  Double_t *px = s->GetPositionX();
  Double_t *py = s->GetPositionY();
  for (auto i = 0; i < s->GetNPeaks(); ++i)
    std::cout << "Peak[" << i << "]:  " << px[i] << ";" << std::endl;// << "y = " << py[i] << ";" << std::endl;
  
  
  auto legend = new TLegend(0.1,0.7,0.48,0.9);
  legend->AddEntry(h,"Original spectra");
  legend->AddEntry(pm,"alg1 (44 peaks found)"); //root
  legend->AddEntry(pmPD,"alg2 (57 peaks found)"); // peakdeco
  legend->AddEntry(pmG,"Genie (53 peaks found)"); // peakdeco
  //legend->AddEntry(grGenie,"genie");
  legend->Draw("same");
  
}
/**
 * References:
 * [1] M. Morháč, J. Kliman, V. Matoušek, M. Veselský, I. Turzo.: Background elimination methods for multidimensional gamma-ray spectra. NIM, A401 (1997) 113-132.
 */


TPolyMarker* SearchPeaksPD(const char *fileName) {
  std::ifstream iFile(fileName);
  std::string st;
  if (!iFile) return nullptr;
  std::string temp;
  std::vector<double> energy;      
  std::vector<double> counts;      
  double en = 0.;
  double cnt = 0.;
  double enPrev = 0.;
  bool flag = true;
  while(std::getline(iFile, st)) {
    std::istringstream iss{st};
    if (st[0] == '#') {
      iss >> temp >> temp >> temp >> temp >> en;
      energy.push_back(en);
      flag = true;
    }
    else if (flag) {
      iss >> en >> cnt;
        counts.push_back(TMath::Log(TMath::Log(TMath::Sqrt(cnt+1)+1)+1));
        flag = false;
    }
  }
  
  auto pm = new TPolyMarker(counts.size(), &energy[0], &counts[0]);
  pm->SetMarkerStyle(22);
  pm->SetMarkerColor(kBlue);
  pm->SetMarkerSize(1.3);
  
  std::cout << "The identified peaks by peakdeco are:" << std::endl;
  for (auto i = 0; i < counts.size(); ++i)
    std::cout << "Peak[" << i << "]:  " << energy[i] << ";" << std::endl;
  
  return pm;
  
}

TPolyMarker* SearchPeaksGenie(const char *fileName) {
  std::ifstream iFile(fileName);
  std::string st;
  if (!iFile) return nullptr;
  std::string temp;
  std::vector<double> energy;      
  std::vector<double> counts;      
  double n, chan, err, en, cnts;
  while(std::getline(iFile, st)) {
    std::istringstream iss{st};
    iss >> n >> chan>> err>> en >> cnts;
    if (cnts > 10) {
      energy.push_back(en);
     // counts.push_back(TMath::Log(TMath::Log(TMath::Sqrt(cnts+1)+1)+1)+0.6);
      counts.push_back(1.7);
    }
  }
  
  auto pm = new TPolyMarker(counts.size(), &energy[0], &counts[0]);
  pm->SetMarkerStyle(24);
  pm->SetMarkerColor(6);
  pm->SetMarkerSize(1.3);
  
  std::cout << "The identified peaks by Genie are:" << std::endl;
  for (auto i = 0; i < counts.size(); ++i)
    std::cout << "Peak[" << i << "]:  " << energy[i] << ";" << std::endl;
  
  return pm;
  
}
