#include "TH1.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TRandom3.h"

void AddEmissions(TH1D* h, int emmissionsCount);
void AddNoise(TH1D* h, double noiseLevel);
TH1D* GenHist(int n, double noiseLevel, int emmissionsCount);
TGraph* GenGraph(int n, double noiseLevel, int emmissionsCount);
void DataGen();

void AddNoise(TH1D* h, double noiseLevel=0.3) {
  for (auto i = 0; i < h->GetNbinsX(); ++i)
    h->SetBinContent(i, h->GetBinContent(i) + noiseLevel*h->GetMaximum()*gRandom->Uniform());
}

void AddEmissions(TH1D* h, int emmissionsCount=4) {
  int ne{};
  for (auto i = 0; i<emmissionsCount; ++i) {  
    ne = (int) (h->GetNbinsX()*gRandom->Uniform());
    h->SetBinContent(ne, h->GetBinContent(ne) + 0.7*h->GetMaximum());
  }

}

TH1D* GenHist(int n = 30, double noiseLevel=0.3, int emissionsCount = 4) {
  auto h = new TH1D("hist", "hist", n, -5, 5);
  h->FillRandom("gaus", 10000);
  AddNoise(h, noiseLevel);
  AddEmissions(h, emissionsCount);
  return h;
}

TGraph* GenGraph(int n = 30, double noiseLevel=0.3, int emissionsCount = 4) {
  auto h = GenHist(n, noiseLevel, emissionsCount);
  auto gr = new TGraph(n);
  for (auto i = 0; i < h->GetNbinsX(); ++i)
      gr->SetPoint(i,i,h->GetBinContent(i));

  return gr;
}

void DataGen() {
  auto h =  GenHist(100);
  auto c1 = new TCanvas();
  c1->cd();
  h->DrawClone();
  auto c2 = new TCanvas();
  c2->cd();
  GenGraph(100)->DrawClone("ap");

}