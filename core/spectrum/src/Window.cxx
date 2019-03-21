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
#include "Utilities.h"

SpectrumId::Window::Window(const std::vector<double>& val, int currentIndex, int windowSize): svData(val), sCurrentIndex(currentIndex), sWindowSize(windowSize)
{
    ::Info("spectrumId::Window::Window", "Start creating object");
    if (val.empty()) throw std::invalid_argument("Input data for Window is empty");
    sxErr = std::vector<double>(val.size(), 0);
    sxData = std::vector<double>(val.size());
    std::iota(std::begin(sxData), std::end(sxData),0);
    svErr = std::vector<double>(val.size(), 0);
    for (auto i = 0; i < val.size(); ++i)
      svErr[i] = TMath::Sqrt(val[i]); // if (val[i] != 0) svErr[i] = 1.0 / val[i];
    
    if (sWindowSize < 3) {
      ::Warning("Window::Window", "Size of window too small. 3 will be set.");
      sWindowSize = 3;
    }
    
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
    
    sWindowArrayX = std::vector<double>(&sxData[startIndex], &sxData[endIndex]);
    sWindowArrayXExceptCurrentPoint  = sWindowArrayX;
    sWindowArrayXExceptCurrentPoint.erase(sWindowArrayXExceptCurrentPoint.begin()+sCurrentIndexInWindow);
    
    
    sWindowArrayXErr = std::vector<double>(&sxErr[startIndex], &sxErr[endIndex]);
    sWindowArrayXErrExceptCurrentPoint = sWindowArrayXErr;
    sWindowArrayXErrExceptCurrentPoint.erase(sWindowArrayXErrExceptCurrentPoint.begin() + sCurrentIndexInWindow);
    

    sWindowArrayVal = std::vector<double>(&svData[startIndex], &svData[endIndex]);
    sWindowArrayValExceptCurrentPoint = sWindowArrayVal;
    sWindowArrayValExceptCurrentPoint.erase(sWindowArrayValExceptCurrentPoint.begin() + sCurrentIndexInWindow);
    
    sWindowArrayValErr = std::vector<double>(&svErr[startIndex], &svErr[endIndex]);
    sWindowArrayValErrExceptCurrentPoint = sWindowArrayValErr;
    sWindowArrayValErrExceptCurrentPoint.erase(sWindowArrayValErrExceptCurrentPoint.begin() + sCurrentIndexInWindow);
    
    ::Info("spectrumId::Window", "::Window(\n%s,\n%s,\n%s,\n%s,\n%d, %d). Current value is %f", \
                                           Utilities::Vec2String(sWindowArrayX).c_str(),\
                                           Utilities::Vec2String(sWindowArrayVal).c_str(),\
                                           Utilities::Vec2String(sWindowArrayXErr).c_str(),\
                                           Utilities::Vec2String(sWindowArrayValErr).c_str(),\
                                           currentIndex, windowSize, sCurrentPointVal);
    
    ::Info("spectrumId::Window", "WindowArrayExceptCurrentPoint:\n%s\n%s\n%s\n%s\n", \
                                           Utilities::Vec2String(sWindowArrayXExceptCurrentPoint).c_str(),\
                                           Utilities::Vec2String(sWindowArrayValExceptCurrentPoint).c_str(),\
                                           Utilities::Vec2String(sWindowArrayXErrExceptCurrentPoint).c_str(),\
                                           Utilities::Vec2String(sWindowArrayValErrExceptCurrentPoint).c_str());
    
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
  
    ROOT::Fit::BinData bData(sWindowArrayXExceptCurrentPoint.size(), sWindowArrayXExceptCurrentPoint.data(), sWindowArrayValExceptCurrentPoint.data(), sWindowArrayXErrExceptCurrentPoint.data(), sWindowArrayValErrExceptCurrentPoint.data());
    
    auto f1 = new TF1(curve.c_str(), curve.c_str());
    ROOT::Math::WrappedMultiTF1 ff(*f1,f1->GetNdim());
    ROOT::Fit::Fitter fitter;
    fitter.SetFunction(ff,false);
    
    fitter.Fit(bData);
    
    auto r = fitter.Result();
//     r.Print(std::cout);
    
    bool isFitted = true;
    
    if (r.Chi2() > 1) isFitted = false;
    
    auto fitPars = r.Parameters();
    sCurrentFitPoint = 0.0;
    
    for (auto& fi : fitPars) sCurrentFitPoint += fi*TMath::Power(sCurrentPointX, double(&fi - &fitPars[0])); 
    
    if ((1.0/sCurrentPointVal) * (sCurrentPointVal - sCurrentFitPoint) * (sCurrentPointVal - sCurrentFitPoint) >= eps && isFitted) return true;
    
    return false;
}

