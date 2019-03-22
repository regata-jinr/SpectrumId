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


void SpectrumId::Window::FormArrays(std::vector<double>& origArray, std::vector<double>& newArray, std::vector<double>& modifiedArray) {
    int startIndex = sCurrentIndex - sCurrentIndexInWindow;
    int endIndex = sCurrentIndex + sCurrentIndexInWindow + 1;
    
    //boundary points
    if (sCurrentIndex < sCurrentIndexInWindow) {
      startIndex = 0;
      endIndex = sWindowSize;
      sCurrentIndexInWindow = sCurrentIndex;
    }
    if (sCurrentIndex >= svData.size() - sCurrentIndexInWindow) {
      startIndex = svData.size() - sWindowSize;
      endIndex = svData.size();
      sCurrentIndexInWindow = sWindowSize - (svData.size() - sCurrentIndex - 1) - 1;
    }
    //boundary points  
    
    newArray = std::vector<double>(&origArray[startIndex], &origArray[endIndex]);
    modifiedArray  = newArray;
    modifiedArray.erase(modifiedArray.begin()+sCurrentIndexInWindow);
}


/**
* @brief Constructor of window
* Window is subset of input array with specified size.
* @param val - vector of input data
* @param currentIndex - which point from input data should be a center of window
* @param windowSize - size of window. 5 by default.
*/
SpectrumId::Window::Window(const std::vector<double>& val, int currentIndex, int windowSize): svData(val), sCurrentIndex(currentIndex), sWindowSize(windowSize)
{
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
    
    sCurrentPointVal      = svData[currentIndex];
    sCurrentPointX        = sxData[currentIndex];
    sCurrentPointErrX     = sxErr[currentIndex];
    sCurrentPointErrVal   = svErr[currentIndex];
    sCurrentIndexInWindow = windowSize / 2;
    sCurrentFitPoint      = 0;
    
    SpectrumId::Window::FormArrays(sxData, sWindowArrayX, sWindowArrayXExceptCurrentPoint);
    SpectrumId::Window::FormArrays(sxErr, sWindowArrayXErr, sWindowArrayXErrExceptCurrentPoint);
    SpectrumId::Window::FormArrays(svData, sWindowArrayVal, sWindowArrayValExceptCurrentPoint);
    SpectrumId::Window::FormArrays(svErr, sWindowArrayValErr, sWindowArrayValErrExceptCurrentPoint);
    
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

/**
* @brief Checks current point on emission
* The idea of this checking that emission breaks continuity of curve describing spectrum. 
* @param curve - name of TF1 function which fit window array without current point.
* @param eps - value for comparison with chi2 of current point and fitVal of current point
* @return true if chi2 of current point and fit point more than eps and window could be fit of curve, current point is emission
*/
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
    
    for (auto& fi : fitPars) sCurrentFitPoint += fi*TMath::Power(sCurrentPointX, double(&fi - &fitPars[0]));
    
    if ((1.0/sCurrentPointVal) * (sCurrentPointVal - sCurrentFitPoint) * (sCurrentPointVal - sCurrentFitPoint) >= eps && isFitted) return true;
    
    return false;
}

/**
* @brief Compares current value with neighbors...
*        If current point more than c*(value[i-1] + value[i+1])/ 2 likely it's emission.
* @param c - scale factor
* @return true if current point more than c*(value[i-1] + value[i+1])/ 2
*/
bool SpectrumId::Window::IsMoreThanCNeighbors(double c) {
  double leftValue{}, rightValue{};
  if (sCurrentIndexInWindow > 0)
    leftValue = sWindowArrayVal[sCurrentIndexInWindow - 1];
  else return (sCurrentPointVal > (c*sWindowArrayVal[sCurrentIndexInWindow + 1]));
  
  if (sCurrentIndexInWindow < (sWindowSize - 1)) 
    rightValue = sWindowArrayVal[sCurrentIndexInWindow + 1];
  else return (sCurrentPointVal > (c*sWindowArrayVal[sCurrentIndexInWindow - 1]));

  return (sCurrentPointVal > ((c / 2) * (rightValue + leftValue)));
}


