#include <gtest/gtest.h>

#include "TCanvas.h"
#include "TMath.h"
#include "TH1.h"
#include <iostream>
#include "Spectrum.h"


void TryIt() {
  auto *his = new TH1D("his", "his",1000,-100,100);
  his->FillRandom("gaus",100000);
  his->Draw();
}
