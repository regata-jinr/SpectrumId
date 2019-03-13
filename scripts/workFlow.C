#include "TError.h"
#include "TH1.h"
#include "TGraph.h"
#include <vector>
#include <TROOT.h>
#include <TPad.h>
#include <TRandom3.h>

void PeakNoiser(TH1D* h, int a);
void workFlow(int n);
void CutEmissions(TH1D* h);

void PeakNoiser(TH1D* h, int a = 50) {
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
}


//TODO: 

void CutEmissions(TH1D* h) {
  int n = h->GetNbinsX();
  auto dat = h->GetArray();

  
  
  delete dat;
}

void workFlow(int n = 150) {
  auto hp = new TH1D("hp", "hp", n, -5,5);
  hp->FillRandom("gaus",n*20);
  auto hpN = new TH1D(*hp);
  PeakNoiser(hpN);
   
  hpN->SetLineColor(2);
  hp->SetMaximum(300);
  hp->DrawClone("e1");
  hpN->DrawClone("e1same");
  
}
