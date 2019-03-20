/***************************************************************************
 *                                                                         *
 *                                                                         *
 * Copyright(c) 2018-2019, REGATA Experiment at JINR, All rights reserved. *
 *                                                                         *
 *                                                                         *
 ***************************************************************************/

#include "Window.h"
#include <stdexcept>
#include <numeric>
#include "TError.h"
#include "TF1.h"
#include "Fit/BinData.h"
#include "Fit/Fitter.h"
#include "Math/WrappedMultiTF1.h"
#include "TMath.h"

SpectrumId::Window::Window(const std::vector<double>& val, int currentIndex, int windowSize): svData(val), sCurrentIndex(currentIndex), sWindowSize(windowSize)
{
    ::Info("spectrumId::Window::Window", "Start creating object");
    if (val.empty()) throw std::invalid_argument("Input data for Window is empty");
    sxErr = std::vector<double>(val.size(), 0);
    sxData = std::vector<double>(val.size());
    std::iota(std::begin(sxData), std::end(sxData),0);
    svErr = std::vector<double>(val.size(), 0);
    for (auto i = 0; i < val.size(); ++i)
      if (val[i] != 0) svErr[i] = TMath::Sqrt(val[i]);//1.0 / val[i];
    
    if (sWindowSize < 3) {
      ::Warning("Window::Window", "Size of window too small. 3 will be set.");
      sWindowSize = 3;
    }
    
     ::Info("spectrumId::Window::Window", "Fill window values:");
    
    sCurrentPointVal = svData[currentIndex];
    sCurrentPointX = sxData[currentIndex];
    sCurrentPointErrX = sxErr[currentIndex];
    sCurrentPointErrVal = svErr[currentIndex];
    sCurrentIndexInWindow = windowSize / 2;
    
    int startIndex = currentIndex - sCurrentIndexInWindow;
    int endIndex = currentIndex + sCurrentIndexInWindow + 1;
    
    //boundary points
    if (currentIndex < sCurrentIndexInWindow) {
      startIndex = 0;
      endIndex = windowSize;
      sCurrentIndexInWindow = currentIndex;
    }
    if (currentIndex >= svData.size() - sCurrentIndexInWindow) {
      startIndex = svData.size() - windowSize;
      endIndex = svData.size();
      sCurrentIndexInWindow = windowSize - (svData.size() - currentIndex - 1) - 1;
    }
    //boundary points   
    
    ::Info("spectrumId::Window::Window", "Fill window arrays indexes: start = %d -- curInWin = %d -- curInArr = %d -- end = %d", startIndex, sCurrentIndexInWindow, currentIndex, endIndex);
    ::Info("spectrumId::Window::Window", "Fill window arrays values: start = %f -- curInWin = %f -- curInArr = %f -- end = %f", svData[startIndex], svData[sCurrentIndexInWindow], svData[currentIndex], svData[endIndex]);
         
    sWindowArrayX = std::vector<double>(&sxData[startIndex], &sxData[endIndex]);
    sWindowArrayXExceptCurrentPoint  = sWindowArrayX;
    sWindowArrayXExceptCurrentPoint.erase(sWindowArrayXExceptCurrentPoint.begin()+sCurrentIndexInWindow);
    
    ::Info("spectrumId::Window::Window", "X array has filled");
    
    sWindowArrayXErr = std::vector<double>(&sxErr[startIndex], &sxErr[endIndex]);
    sWindowArrayXErrExceptCurrentPoint = sWindowArrayXErr;
    sWindowArrayXErrExceptCurrentPoint.erase(sWindowArrayXErrExceptCurrentPoint.begin() + sCurrentIndexInWindow);
    
    ::Info("spectrumId::Window::Window", "XErr array has filled");

    sWindowArrayVal = std::vector<double>(&svData[startIndex], &svData[endIndex]);
    sWindowArrayValExceptCurrentPoint = sWindowArrayVal;
    sWindowArrayValExceptCurrentPoint.erase(sWindowArrayValExceptCurrentPoint.begin() + sCurrentIndexInWindow);
    
    sWindowArrayValErr = std::vector<double>(&svErr[startIndex], &svErr[endIndex]);
    sWindowArrayValErrExceptCurrentPoint = sWindowArrayValErr;
    sWindowArrayValErrExceptCurrentPoint.erase(sWindowArrayValErrExceptCurrentPoint.begin() + sCurrentIndexInWindow);
    
}

SpectrumId::Window::Window(const std::vector<double>& x, const std::vector<double>& val, int currentIndex, int windowSize): SpectrumId::Window::Window(val, currentIndex, windowSize)
{
    if (val.empty() || x.empty()) throw std::invalid_argument("Input data for Window is empty");
    sxErr = std::vector<double>(val.size(), 0);
    svErr = std::vector<double>(val.size());
    for (auto i = 0; i < val.size(); ++i)
      svErr[i] = 1.0 / val[i];
}

SpectrumId::Window::Window(const std::vector<double>& x, const std::vector<double>& val, const std::vector<double>& xErr, const std::vector<double>& valErr, int currentIndex, int windowSize):  SpectrumId::Window::Window(val, currentIndex, windowSize)
{
  if (val.empty() || x.empty() || xErr.empty() || valErr.empty()) throw std::invalid_argument("Input data for Window is empty");
    sxErr = xErr;
    sxData = x;
    svErr = valErr;
}

bool SpectrumId::Window::IsCurrentValueEmission(std::string curve, double eps) {
  
    std::cout << "X array for fitting:" << std::endl;
    for(auto &vi : sWindowArrayXExceptCurrentPoint) std::cout << vi << "; ";
    std::cout << std::endl;
  
    std::cout << "Values array for fitting:" << std::endl;
    for(auto &vi : sWindowArrayValExceptCurrentPoint) std::cout << vi << "; ";
    std::cout << std::endl;
    ROOT::Fit::BinData bData(sWindowArrayXExceptCurrentPoint.size(), sWindowArrayXExceptCurrentPoint.data(), sWindowArrayValExceptCurrentPoint.data(), sWindowArrayXErrExceptCurrentPoint.data(), sWindowArrayValErrExceptCurrentPoint.data());
    auto f1 = new TF1("f1", curve.c_str());
    ROOT::Math::WrappedMultiTF1 ff(*f1,f1->GetNdim());
    ROOT::Fit::Fitter fitter;
   // fitter.Config().SetMinimizer("linear");
    fitter.SetFunction(ff,false);
    fitter.LeastSquareFit(bData);
    auto r = fitter.Result();
    r.Print(std::cout);
    if (!r.IsValid()) return false;
    auto fitPars = r.Parameters();
    sCurrentFitPoint = 0.0;
    for (auto& fi : fitPars) sCurrentFitPoint += fi*TMath::Power(sCurrentPointX, double(&fi - &fitPars[0])); 
    ::Info("spectrumId::Window", "IsCurrentValueEmission(%s, %f); fitVal = %f, currVal = %f, chi2 = %f, FitIsValid = %d", curve.c_str(), eps,  sCurrentFitPoint, sCurrentPointVal, (1.0/sCurrentPointVal) * (sCurrentPointVal - sCurrentFitPoint) * (sCurrentPointVal - sCurrentFitPoint), r.IsValid());
    if ((1.0/sCurrentPointVal) * (sCurrentPointVal - sCurrentFitPoint) * (sCurrentPointVal - sCurrentFitPoint) >= eps) return true;
    return false;
}

