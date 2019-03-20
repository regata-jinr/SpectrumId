#include "TError.h"
#include "TH1.h"
// #include "TF1.h"
// #include "TGraph.h"
#include <vector>
#include <iostream>
// #include "TROOT.h"
// #include "TPad.h"
#include "TRandom3.h"
#include "TSpectrum.h"
// #include "TMath.h"
#include "Window.h"
#include "Fit/BinData.h"
#include "Fit/Fitter.h"
#include "Math/WrappedMultiTF1.h"

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
 gSystem->AddIncludePath("-I/home/bdrum/GoogleDrive/Job/flnp/srec/SpectrumId/inc")
 .L ../src/Window.cxx+
 .L workFlow.C+
 workFlow();
 
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
  
  std::vector<double> v = {0,1,4,9,16,25,36,490,64,91};
  std::vector<double> tv = {0,1,4,9,16,25,36,490,64,91};
  

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


void testFit() {
  std::vector<double> x {5, 6, 8, 9};
  std::vector<double> xE(4,0);
  std::vector<double> y {25, 36, 64, 91 };
  std::vector<double> yE {5, 6, 8, 9};//{1./25, 1./36, 1./64, 1./91 };
  ROOT::Fit::BinData bData(x.size(), x.data(), y.data(), xE.data(), yE.data());
  auto f1 = new TF1("pol2", "pol2");
  ROOT::Math::WrappedMultiTF1 ff(*f1,f1->GetNdim());
  ROOT::Fit::Fitter fitter;
  fitter.Config().SetMinimizer("Linear");
  fitter.SetFunction(ff);
  fitter.LeastSquareFit(bData);
  auto r = fitter.Result();
  r.Print(std::cout);

}
