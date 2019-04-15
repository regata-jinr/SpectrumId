#include "TError.h"
#include "TH1F.h"
#include "TF1.h"
// #include "TGraph.h"
#include "TFile.h"
#include <vector>
#include <iostream>
#include <iterator>
#include <numeric>
#include "TROOT.h"
#include "TPad.h"
#include "TStyle.h"
#include "TRandom3.h"
#include "TSpectrum.h"
#include "TSystem.h"
#include "DataGen.C"
#include "Utilities.h"
#include "Window.h"


/*
 gSystem->AddIncludePath("-I/home/bdrum/GoogleDrive/Job/flnp/srec/dev/spectrumId/core/spectrum/inc")
 .L /home/bdrum/GoogleDrive/Job/flnp/srec/dev/spectrumId/core/spectrum/src/Window.cxx+
 .L /home/bdrum/GoogleDrive/Job/flnp/srec/dev/spectrumId/core/spectrum/src/Utilities.cxx+
 .L Smoothing.C+
 // Smoothing();
  SimpleArraysSmoothing();
 
*/
void SmoothArray(std::vector<double>& arr);
void SmoothingReal();
void Smoothing();
void SimpleArraysSmoothing(int p);
void SimpleHistSmoothing();

// TODO: Create interface for smoothing functionality
void SmoothArray(std::vector<double>& arr) {
  Utilities::Fitter::curve = "pol2";
  auto w = SpectrumId::Window(arr,0);
  for (auto & aI : arr) {
    w = SpectrumId::Window(arr, &aI - &arr[0]);
    ::Info("SmoothArray", "Current index in window: %d", w.GetIndexInWindow());
    ::Info("SmoothArray", "Window array: %s", Utilities::Vec2String(w.GetWindowArray()).c_str());
    if (Utilities::Fitter::IsEmission(w.GetWindowArray(), w.GetIndexInWindow())) aI = Utilities::Fitter::currentFitPoint;
  }
}

void SimpleArraysSmoothing(int p = 2) {
  std::vector<double> vOrig(12,0);
  std::iota(vOrig.begin(), vOrig.end(), 0);
  for (auto &v : vOrig)
    v = gRandom->Gaus();
    // v = TMath::Power(v,p);
  vOrig[7] += 1000;
  std::vector<double> vZlokazov = vOrig;
  std::vector<double> vTH1 = vOrig;
  std::vector<double> vMarkov = vOrig;
  
  std::cout << "Smoothing comparison of simple arrays" << std::endl << "based on 3 algorithms:" << std::endl;

  SmoothArray(vZlokazov);

  TH1::SmoothArray(vTH1.size(), vTH1.data());

  auto ts = new TSpectrum();
  ts->SmoothMarkov(vMarkov.data(), vMarkov.size(),3);

  ::Info("", "|Orig vector | Zlokazov | TH1::SmoothArray | TSpectrum::SmoothMarkov|");
  ::Info("", "|------------|----------|------------------|------------------------|");
  for (auto i = 0; i < vOrig.size(); ++i)
    ::Info("", "|  %.3f   |   %.3f   |   %.3f   |   %.3f   |", vOrig[i], vZlokazov[i], vTH1[i], vMarkov[i]);
  
}

//TODO: add more special cases 

void Smoothing() {
  SimpleHistSmoothing();

}

void SmoothingReal() {

  SimpleArraysSmoothing();
  Int_t i;
  const Int_t nbins = 1024;
  auto c1 = new TCanvas("c1", "c1");
  c1->cd();
  Double_t sourceOrig[nbins];
  Double_t sourceTH1[nbins];
  Double_t sourceMarkov[nbins];
  std::vector<Double_t> sourceZlkzv(sourceOrig,sourceOrig + nbins);
   
  gROOT->ForceStyle();

  TString dir  = gROOT->GetTutorialDir();
  TString file = dir+"/spectrum/TSpectrum.root";
  TFile *f     = new TFile(file.Data());
  TH1F *h = (TH1F*) f->Get("back1");
  h->SetTitle("Original spectrum");

  for (i = 0; i < nbins; i++) sourceOrig[i]=h->GetBinContent(i + 1);
  for (i = 0; i < nbins; i++) sourceTH1[i]=h->GetBinContent(i + 1);
  for (i = 0; i < nbins; i++) sourceMarkov[i]=h->GetBinContent(i + 1);
  for (i = 0; i < nbins; i++) sourceZlkzv[i]=h->GetBinContent(i + 1);
   
  h->SetAxisRange(1,1024);
  h->DrawClone("L");

  TSpectrum *s = new TSpectrum();

  TH1F *smoothMarkov = new TH1F("smoothMarkov","smoothMarkov",nbins,0.,nbins);
  smoothMarkov->SetLineColor(kRed);

  s->SmoothMarkov(sourceMarkov,1024,3);
  for (i = 0; i < nbins; i++) smoothMarkov->SetBinContent(i + 1,sourceMarkov[i]);
  smoothMarkov->DrawClone("L SAME");
  
  TH1F *smoothTH1 = new TH1F("smoothTH1","smoothTH1",nbins,0.,nbins);
  smoothTH1->SetLineColor(kBlack);
  TH1::SmoothArray(1024,sourceTH1);
  for (i = 0; i < nbins; i++) smoothTH1->SetBinContent(i + 1,sourceTH1[i]);
  smoothTH1->DrawClone("L SAME");
   
  TH1F *smoothZlkzv = new TH1F("smoothZlkzv","smoothZlkzv",nbins,0.,nbins);
  smoothZlkzv->SetLineColor(kGreen);
  SmoothArray(sourceZlkzv);
  for (i = 0; i < nbins; i++) smoothZlkzv->SetBinContent(i + 1,sourceZlkzv[i]);
  smoothZlkzv->DrawClone("L SAME");

  gStyle->SetOptStat(0);
  gPad->BuildLegend();

}

void SimpleHistSmoothing() {

  auto c2 = new TCanvas("c2", "c2");
  c2->cd();

  auto hOrig = GenHist();
  auto smoothMarkovHist = new TH1D(*hOrig);
  auto smoothTH1Hist = new TH1D(*hOrig);
  auto smoothZlkzvHist = new TH1D(*hOrig);
  std::vector<double> vOrig(&hOrig->GetArray()[0], &hOrig->GetArray()[0] + hOrig->GetNbinsX());

  hOrig->SetName("Orig");
  smoothMarkovHist->SetName("Markov");
  smoothZlkzvHist->SetName("Zlkzv");
  smoothTH1Hist->SetName("TH1");
  hOrig->SetTitle("Orig");
  smoothMarkovHist->SetTitle("Markov");
  smoothZlkzvHist->SetTitle("Zlkzv");
  smoothTH1Hist->SetTitle("TH1");

  hOrig->SetLineColor(kBlue);
  smoothMarkovHist->SetLineColor(kRed);
  smoothTH1Hist->SetLineColor(kBlack);
  smoothZlkzvHist->SetLineColor(kGreen);
  hOrig->DrawClone("L");

  TSpectrum *s = new TSpectrum();
  s->SmoothMarkov(smoothMarkovHist->GetArray(),smoothMarkovHist->GetNbinsX(),3);
  smoothMarkovHist->DrawClone("L SAME");
  smoothTH1Hist->Smooth();
  std::vector<double> vTH1(&smoothTH1Hist->GetArray()[0], &smoothTH1Hist->GetArray()[0] + hOrig->GetNbinsX());
  smoothTH1Hist->DrawClone("L SAME");
  std::vector<double> vMarkov(&smoothMarkovHist->GetArray()[0], &smoothMarkovHist->GetArray()[0] + hOrig->GetNbinsX());
   
//TODO: check filling of smoothZlkzvHist

  std::vector<Double_t> source(&smoothZlkzvHist->GetArray()[0], &smoothZlkzvHist->GetArray()[0] + smoothZlkzvHist->GetNbinsX());
  std::vector<Double_t> w(source.size(), 1);
  SmoothArray(source);
  smoothZlkzvHist->Clear();
  smoothZlkzvHist->FillN(source.size(), source.data(), w.data());
 // smoothZlkzvHist->DrawClone("L SAME");
  std::vector<double> vZlokazov(&smoothZlkzvHist->GetArray()[0], &smoothZlkzvHist->GetArray()[0] + hOrig->GetNbinsX());
  gStyle->SetOptStat(0);

  gPad->BuildLegend();

  ::Info("Pol2", "");
  ::Info("", "|Orig vector | Zlokazov | TH1::SmoothArray | TSpectrum::SmoothMarkov|");
  ::Info("", "|------------|----------|------------------|------------------------|");
  for (auto i = 0; i < vOrig.size(); ++i)
    ::Info("", "|  %.3f   |   %.3f   |   %.3f   |   %.3f   |", vOrig[i], vZlokazov[i], vTH1[i], vMarkov[i]);

  // delete c2;
  delete hOrig;
  delete smoothMarkovHist;
  delete smoothTH1Hist;
  delete smoothZlkzvHist;
}
