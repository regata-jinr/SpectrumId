#include "TError.h"
#include "TH1.h"
#include "TF1.h"
// #include "TGraph.h"
#include "TFile.h"
#include <vector>
#include <iostream>
#include "TROOT.h"
#include "TPad.h"
#include "TStyle.h"
#include "TRandom3.h"
#include "TSpectrum.h"
// #include "TMath.h"
#include "Window.h"

void AddNoiseAndEmissions(TH1D* h, int a);
void workFlow(int n);
TH1D* GenHist();

void AddNoiseAndEmissions(TH1D* h, int a = 50) {
  ::Info("", "Nbins is: %d", h->GetNbinsX());

  int b = a;
  // 3 random bins
  int b1 = (int) (h->GetNbinsX()*gRandom->Uniform());
  int b2 = (int) (h->GetNbinsX()*gRandom->Uniform());
  int b3 = (int) (h->GetNbinsX()*gRandom->Uniform());
  
  ::Info("", "Random bins are:{%d, %d, %d}", b1, b2, b3);
  
  for (auto i = 0; i < h->GetNbinsX(); ++i) {
    if (i == b1 || i == b2 || i == b3) b = 5*a;
    h->SetBinContent(i, h->GetBinContent(i) + b*(gRandom->Uniform()+0.3));
    b = a;
  }
  h->SetLineColor(2);
}

TH1D* GenHist() {
  auto h = new TH1D("hist", "hist", 50, -5, 5);
  h->FillRandom("gaus");
  return h;
}


/*
 gSystem->AddIncludePath("-I/home/bdrum/GoogleDrive/Job/flnp/srec/dev/spectrumId/core/spectrum/inc")
 .L /home/bdrum/GoogleDrive/Job/flnp/srec/dev/spectrumId/core/spectrum/src/Window.cxx+
 .L workFlow.C+
 workFlow();
 //Smoothing();
 
*/


void SmoothArray(std::vector<double>& arr) {
  auto w = new SpectrumId::Window(arr,0);
  for (auto & aI : arr) {
    w = new SpectrumId::Window(arr, &aI - &arr[0]);
    if (!w->IsCurrentValueEmission("pol2", 1.0)) aI = w->GetFitPointValue();
  }
}

void workFlow(int n = 150) {
  
  auto h = GenHist();
  h->DrawClone("e1");
  
  auto hn = h;
  
  AddNoiseAndEmissions(hn);
  hn->Smooth();
  hn->DrawClone("e1same");
  
  std::vector<double> v = {0,1,4,9,16,25,36,490,64,91,100,121,144,169,196,225};
  std::vector<double> tv = {0,1,4,9,16,25,36,490,64,91,100,121,144,169,196,225};
  

  SmoothArray(tv);
  
  std::cout << "Orig vector" << std::endl;
  for(auto &vi : v) std::cout << vi << "; ";
  std::cout << std::endl;
  
  std::cout << "My smooth" << std::endl;
  for(auto &vi : tv) std::cout << vi << "; ";
  std::cout << std::endl;
  
  tv =v ;
  
  TH1::SmoothArray(tv.size(), tv.data());
  std::cout << "TH1::SmoothArray" << std::endl;
  for(auto &vi : tv) std::cout << vi << "; ";
  std::cout << std::endl;
  
  tv =v ;
  
  auto ts = new TSpectrum();
  ts->SmoothMarkov(tv.data(), tv.size(),3);
  std::cout << "TSpectrum::SmoothMarkov" << std::endl;
  for(auto &vi : tv) std::cout << vi << "; ";
  std::cout << std::endl;
  
}

//TODO: add more special cases 

void Smoothing() {
   Int_t i;
   const Int_t nbins = 1024;

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
   h->Draw("L");

   TSpectrum *s = new TSpectrum();

   TH1F *smoothMarkov = new TH1F("smoothMarkov","smoothMarkov",nbins,0.,nbins);
   smoothMarkov->SetLineColor(kRed);

   s->SmoothMarkov(sourceMarkov,1024,3);
   for (i = 0; i < nbins; i++) smoothMarkov->SetBinContent(i + 1,sourceMarkov[i]);
   smoothMarkov->Draw("L SAME");
   
   TH1F *smoothTH1 = new TH1F("smoothTH1","smoothTH1",nbins,0.,nbins);
   smoothTH1->SetLineColor(kBlack);
   TH1::SmoothArray(1024,sourceTH1);
   for (i = 0; i < nbins; i++) smoothTH1->SetBinContent(i + 1,sourceTH1[i]);
   smoothTH1->Draw("L SAME");
   
  
   
   TH1F *smoothZlkzv = new TH1F("smoothZlkzv","smoothZlkzv",nbins,0.,nbins);
   smoothZlkzv->SetLineColor(kGreen);
   SmoothArray(sourceZlkzv);
   for (i = 0; i < nbins; i++) smoothZlkzv->SetBinContent(i + 1,sourceZlkzv[i]);
   smoothZlkzv->Draw("L SAME");

   gStyle->SetOptStat(0);
   gPad->BuildLegend();
}
