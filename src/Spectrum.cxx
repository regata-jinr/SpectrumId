/**************************************************************************
 *                                                                        *
 *                                                                        *
 * Copyright(c) 2018, REGATA Experiment at JINR, All rights reserved.     *
 *                                                                        *
 *                                                                        *
 **************************************************************************/

#include "Spectrum.h"

#include "TCanvas.h"
#include "TMath.h"
#include "TH1.h"
#include "TSpectrum.h"
#include "TPolyMarker.h"
#include <fstream>
#include <iostream>

/**
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
  int num;

  while (getline(fs,st)) {
    if (st[0] == '#') continue;
    num=0;
    while ((pos = st.find("\t")) != std::string::npos) {
      token = st.substr(0, pos);
      data[columns[num]].push_back(std::stod(token));
      st.erase(0, pos + 1);
      num++;
    }
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
  auto his = new TH1D("energy", "energy", data["energy(keV)"].size() - 1, &data["energy(keV)"][0]);

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
  Double_t source[nbins], dest[nbins];
  for (auto i = 0; i < nbins; i++) source[i]=h->GetBinContent(i + 1);

  TCanvas *c1 = new TCanvas("c1","c1",10,10,1000,900);
  c1->cd();

  h->Draw();
  s->Background(h, 50,"backorder4 same compton");

  //s->Background(h, 50,"backsmoothing15 compton backorder8 same");
}
//"/home/bdrum/GoogleDrive/Job/flnp/srec/spectra/7105423.txt"
void SearchPeaks(const char *fileName, Double_t sig=2, Option_t *opt="", Double_t treshld=0.05, Int_t iterations=3, Int_t averW=3) {
  auto h = FillHist(fileName, kTRUE, kTRUE);
  TSpectrum *s = new TSpectrum();
  s->SetAverageWindow(averW);
  s->SetDeconIterations(iterations);
  s->Search(h,sig, opt, treshld);

  TPolyMarker * pm = (TPolyMarker*)h->GetListOfFunctions()->FindObject("TPolyMarker");
  if (pm) {
    h->GetListOfFunctions()->Remove(pm);
    delete pm;
  }
  pm = new TPolyMarker(s->GetNPeaks(), s->GetPositionX(), s->GetPositionY());
  h->GetListOfFunctions()->Add(pm);
  pm->SetMarkerStyle(23);
  pm->SetMarkerColor(kRed);
  pm->SetMarkerSize(1.3);

  h->Draw("L");

  std::cout << "The identified peaks are:" << std::endl;
  Double_t *px = s->GetPositionX();
  Double_t *py = s->GetPositionY();
  for (auto i = 0; i < s->GetNPeaks(); ++i)
    std::cout << "Peak[" << i << "]:  " << px[i] << ";" << std::endl;// << "y = " << py[i] << ";" << std::endl;

}
/**
 * References:
 * [1] M. Morháč, J. Kliman, V. Matoušek, M. Veselský, I. Turzo.: Background elimination methods for multidimensional gamma-ray spectra. NIM, A401 (1997) 113-132.
 */
