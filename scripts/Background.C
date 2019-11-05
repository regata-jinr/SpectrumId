//before .L worflow.C+ add into root
//TODO: add peakdeco background estimation
//TODO: add genie2000 background estimation
/*

gSystem->AddIncludePath("-Iinc/");
.L src/Spectrum.cxx+
.L scripts/workflow.C+

*/
#include "TSpectrum.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TSystem.h"
#include "TMath.h"
#include "../core/spectrum/inc/Spectrum.h"
#include <vector>
#include <valarray>
#include <iostream>
#include "TPolyMarker.h"

Double_t* Normalize(Double_t* arr, Int_t size) {
  std::valarray<Double_t> vad(arr, size);
  Double_t mean = TMath::Mean(size, arr);
  Double_t sigma = TMath::RMS(size, arr);
  std::cout << mean << " " << sigma << std::endl;
  vad = (vad/sigma)-(mean/sigma);
  return &vad[0];
}

void Parse2ColCSV(const char* file, std::vector<double> &a, std::vector<double> &b) {
  std::ifstream is(file);
  double x,y;
  while (is) {
    is >> x >> y;
    a.push_back(x);
    b.push_back(y);
  }
}


void Parse2ColCSVGenie(const char* file, std::vector<double> &a, std::vector<double> &b) {
  std::ifstream is(file);
  double x,y;
  int c,d;
  std::string s;
  while (is) {
    is >> s >> c >> d >> x >> y;
    a.push_back(x);
    b.push_back(y);
  }
}

Double_t* SMNP(Double_t* arr, Int_t size) {
  auto sArr = new double[size];
  for (auto i=0;i<size;++i) sArr[i]=TMath::Log(TMath::Log(TMath::Sqrt(arr[i]+1)+1)+1);
  return sArr;
}


void Background() {
  
}

void BackgroundEstimation() {
  auto sp = new Spectrum("../peakdeco/peakdeco-84/data/check/7105423.txt");
  auto tsp = new TSpectrum();
  auto vic = sp->GetCountsArray();
  auto vde = sp->GetEnergyArray();
  std::vector<double> vde1(vde.begin()+500, vde.begin()+2000);
  std::vector<double> vdc(vic.begin()+500, vic.begin()+2000);
  std::vector<double> vBackPDCounts;
  std::vector<double> vBackPDEnergy;
  Parse2ColCSV("../peakdeco/peakdeco-84/background.dat", vBackPDEnergy, vBackPDCounts);
  std::vector<double> vBackGenCounts;
  std::vector<double> vBackGenEnergy;
  Parse2ColCSVGenie("../peakdeco/peakdeco-84/data/check/backgroundGenie.TXT", vBackGenEnergy, vBackGenCounts);
  std::vector<double> vdc1(vdc);
  Double_t* y = SMNP(vdc.data(), vdc.size());
  Double_t* d = SMNP(vdc1.data(), vdc.size());
  Double_t* b = SMNP(vBackPDCounts.data(), vBackPDCounts.size());
  Double_t* x = &vde1[0];
  Double_t* xb = &vBackPDEnergy[0];
  Double_t* ybg = SMNP(vBackGenCounts.data(), vBackGenCounts.size()); //&vBackGenCounts[0];
  Double_t* xbg = &vBackGenEnergy[0];
  
  
  
  tsp->Background(d,vdc.size(),15, TSpectrum::kBackDecreasingWindow,TSpectrum::kBackOrder2,kFALSE,TSpectrum::kBackSmoothing15,kTRUE);
  
  auto c1 = new TCanvas("c1", "Background estimation");
  c1->cd();
  
  auto grOrig    = new TGraph(vdc.size(), x,y);
  auto grBack    = new TGraph(vdc.size(), x,d);
  auto grPD      = new TGraph(vBackPDEnergy.size(), xb,b);
  auto grGenie   = new TGraph(vBackGenEnergy.size(), xbg,ybg);
  
  grOrig->SetTitle("Background estimation");
  grOrig->SetLineWidth(0);
  grOrig->SetMarkerStyle(8);
  grOrig->SetMarkerSize(0.4);
  grOrig->Draw();
  
  grBack->SetTitle("root");
  grBack->SetLineColor(2);
  grBack->SetLineWidth(2);
  grBack->Draw("same");
  
  grPD->SetTitle("peakdeco");
  grPD->SetLineColor(4);
  grPD->SetLineWidth(2);
  grPD->Draw("same");
  
  grGenie->SetTitle("genie");
  grGenie->SetLineColor(6);
  grGenie->SetLineWidth(2);
  grGenie->Draw("same");
  
  auto legend = new TLegend(0.1,0.7,0.48,0.9);
  legend->AddEntry(grOrig,"Original spectra");
  legend->AddEntry(grBack,"alg1"); // root
  legend->AddEntry(grPD,"alg2");  // peakDeco
  legend->AddEntry(grGenie,"genie");
  legend->Draw("same");
  
}
