#include "TRandom.h"
#include "TH1D.h"
#include "TCanvas.h"
#include <sstream>

TH1D* PoisGen(int k, int n);
void run();

TH1D* PoisGen(int k, int n) {

  auto r = new TRandom();
  
  std::stringstream tit;
  tit << "PoisonDistr(" << k << "). Size=" << n;
  
  auto gr = new TH1D(tit.str().c_str(), tit.str().c_str(), n, -k/2,2*k);
    for (auto i = 0; i < n; ++i)
      gr->Fill(r->PoissonD(k));    
    
    return gr;
}

void run() {
 // auto c1 = new TCanvas("c1", "c1", 900, 500);

  auto gr5 = PoisGen(5, 100);
  gr5->SetLineColor(2);
  gr5->Draw();
  auto gr7 = PoisGen(7, 100);
  gr7->SetLineColor(3);
  gr7->Draw();
  auto gr10 = PoisGen(10, 100);
  gr10->SetLineColor(4);
  gr10->Draw();
}

