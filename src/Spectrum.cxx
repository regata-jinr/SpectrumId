
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

/*
The identified peaks are:
Peak[0]:  60.661;
Peak[1]:  2753.7;
Peak[2]:  69.052;
Peak[3]:  75.045;
Peak[4]:  1596.34;
Peak[5]:  103.415;
Peak[6]:  1691.35;
Peak[7]:  2756.89;
Peak[8]:  559.655;
Peak[9]:  2521.09;
Peak[10]:  2096.09;
Peak[11]:  98.6195;
Peak[12]:  2655.15;
Peak[13]:  1120.8;
Peak[14]:  2110.86;
Peak[15]:  2749.71;
Peak[16]:  1291.71;
Peak[17]:  564.049;
Peak[18]:  1099.64;
Peak[19]:  2083.32;
Peak[20]:  657.519;
Peak[21]:  1787.95;
Peak[22]:  1368.77;
Peak[23]:  2009.88;
Peak[24]:  2757.69;
Peak[25]:  2201.45;
Peak[26]:  1474.97;
Peak[27]:  889.575;
Peak[28]:  2242.96;
Peak[29]:  2347.91;
Peak[30]:  776.947;
Peak[31]:  487.351;
Peak[32]:  1599.13;
Peak[33]:  2899.31;
Peak[34]:  1215.84;
Peak[35]:  1076.88;
Peak[36]:  2411.76;
Peak[37]:  3113.52;
Peak[38]:  1173.91;
Peak[39]:  1317.66;
Peak[40]:  1229.42;
Peak[41]:  816.088;
Peak[42]:  834.86;
Peak[43]:  3099.96;
Peak[44]:  1332.83;
Peak[45]:  311.974;
Peak[46]:  1924.87;
Peak[47]:  1593.94;
Peak[48]:  2507.12;
Peak[49]:  277.615;
Peak[50]:  619.572;
Peak[51]:  1044.53;
Peak[52]:  796.118;
Peak[53]:  496.539;
Peak[54]:  879.591;
Peak[55]:  1177.91;
Peak[56]:  2805.96;
Peak[57]:  2669.91;
Peak[58]:  2793.99;
Peak[59]:  2490.76;
Peak[60]:  1870.98;
Peak[61]:  2614.45;
Peak[62]:  3035.74;
Peak[63]:  3038.93;
Peak[64]:  3044.91;
Peak[65]:  603.994;
Peak[66]:  828.07;
Peak[67]:  228.073;
Peak[68]:  2936.41;
Peak[69]:  1525.28;
Peak[70]:  1732.07;
Peak[71]:  2978.69;
Peak[72]:  2905.29;
Peak[73]:  966.255;
Peak[74]:  116.999;
Peak[75]:  2800.37;
Peak[76]:  698.66;
Peak[77]:  3106.74;
Peak[78]:  2029.04;
Peak[79]:  329.152;
Peak[80]:  693.069;
Peak[81]:  868.009;
Peak[82]:  1221.43;
Peak[83]:  2429.31;
Peak[84]:  1439.44;
Peak[85]:  1189.49;
Peak[86]:  2716.19;
Peak[87]:  925.52;
Peak[88]:  1272.54;
Peak[89]:  140.973;
Peak[90]:  2463.63;
Peak[91]:  3051.69;
Peak[92]:  3061.26;
Peak[93]:  1820.69;
Peak[94]:  2693.45;
Peak[95]:  2814.34;
Peak[96]:  3007.01;
Peak[97]:  2924.44;
Peak[98]:  685.879;
Peak[99]:  3001.43;
*/
