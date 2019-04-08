#include "TError.h"
#include "TH1F.h"
#include "TF1.h"
// #include "TGraph.h"
#include "TFile.h"
#include <vector>
#include <iostream>
#include <iterator>
#include "TROOT.h"
#include "TPad.h"
#include "TStyle.h"
#include "TRandom3.h"
#include "TSpectrum.h"
#include "TSystem.h"
#include "DataGen.C"
#include "../core/spectrum/inc/Window.h"


/*
 gSystem->AddIncludePath("-I/home/bdrum/GoogleDrive/Job/flnp/srec/dev/spectrumId/core/spectrum/inc")
 .L /home/bdrum/GoogleDrive/Job/flnp/srec/dev/spectrumId/core/spectrum/src/Window.cxx+
 .L Smoothing.C+
 Smoothing();
 
*/
void SmoothArray(std::vector<double>& arr);
void SmoothingReal();
void Smoothing();
void SimpleArraysSmoothing();
void SimpleHistSmoothing();

void SmoothArray(std::vector<double>& arr) {
  auto w = new SpectrumId::Window(arr,0);
  for (auto & aI : arr) {
    w = new SpectrumId::Window(arr, &aI - &arr[0]);
    if (!w->IsCurrentValueEmission("pol2", 1.0)) aI = w->GetFitPointValue();
  }
}

void SimpleArraysSmoothing() {
  
  std::vector<double> v = {0,1,4,9,16,25,36,490,64,91,100,121,144,169,196,225};
  std::vector<double> tv = {0,1,4,9,16,25,36,490,64,91,100,121,144,169,196,225};
  
  std::cout << "Smoothing comparison of simple arrays" << std::endl << "based on 3 algorithms:" << std::endl;

  SmoothArray(tv);
  
  std::cout << "Orig vector" << std::endl;
  for(auto &vi : v) std::cout << vi << "; ";
  std::cout << std::endl;
  
  std::cout << "My smooth" << std::endl;
  for(auto &vi : tv) std::cout << vi << "; ";
  std::cout << std::endl;
  
  tv = v ;
  
  TH1::SmoothArray(tv.size(), tv.data());
  std::cout << "TH1::SmoothArray" << std::endl;
  for(auto &vi : tv) std::cout << vi << "; ";
  std::cout << std::endl;
  
  tv = v ;
  
  auto ts = new TSpectrum();
  ts->SmoothMarkov(tv.data(), tv.size(),3);
  std::cout << "TSpectrum::SmoothMarkov" << std::endl;
  for(auto &vi : tv) std::cout << vi << "; ";
  std::cout << std::endl;
  
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
  hOrig->SetLineColor(kBlue);
  smoothMarkovHist->SetLineColor(kRed);
  smoothTH1Hist->SetLineColor(kBlack);
  smoothZlkzvHist->SetLineColor(kGreen);
  hOrig->DrawClone("L");

  TSpectrum *s = new TSpectrum();
  s->SmoothMarkov(smoothMarkovHist->GetArray(),smoothMarkovHist->GetNbinsX(),3);
 // smoothMarkovHist->DrawClone("L SAME");
 // smoothTH1Hist->Smooth();
  smoothTH1Hist->DrawClone("L SAME");
   
  std::vector<Double_t> source(smoothZlkzvHist->GetArray()[0], smoothMarkovHist->GetNbinsX());
  std::vector<Double_t> w(source.size(), 1);
  SmoothArray(source);
  smoothZlkzvHist->Clear();
  smoothZlkzvHist->FillN(source.size(), source.data(), w.data());
  smoothZlkzvHist->DrawClone("L SAME");
  gStyle->SetOptStat(0);
  gPad->BuildLegend();
}
