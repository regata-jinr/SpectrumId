#pragma once

#include <vector>
#include <string>

namespace Utilities {
  template <typename T> 
  std::string Vec2String(const std::vector<T>& v);
 // bool IsCurrentValueEmission(Window w, std::string curve, double eps); 

  template <typename T> 
  std::string Vec2String(const std::vector<T>& v) {
    std::string s = "";
  
    for (const auto & vi: v) {
      if (vi != v.back()) s += std::to_string(vi) + ", ";
      else s += std::to_string(vi);
    }
    return s;
  }

/**
* @brief Checks current point on emission
* The idea of this checking that emission breaks continuity of curve describing spectrum. 
* @param curve - name of TF1 function which fit window array without current point.
* @param eps - value for comparison with chi2 of current point and fitVal of current point
* @return true if chi2 of current point and fit point more than eps and window could be fit of curve, current point is emission
*/
  /*
  bool IsCurrentValueEmission(Window w, std::string curve = "pol2", double eps = 0.15) {
      
    ROOT::Fit::BinData bData(sWindowArrayXExceptCurrentPoint.size(), sWindowArrayXExceptCurrentPoint.data(),\
                             sWindowArrayValExceptCurrentPoint.data(), sWindowArrayXErrExceptCurrentPoint.data(),\
                             sWindowArrayValErrExceptCurrentPoint.data());
    
    auto f1 = new TF1(curve.c_str(), curve.c_str());
    ROOT::Math::WrappedMultiTF1 ff(*f1,f1->GetNdim());
    ROOT::Fit::Fitter fitter;
    fitter.SetFunction(ff,false);
    fitter.Fit(bData);
    auto r = fitter.Result();
    r.Print(std::cout);
    bool isFitted = true;
    if (r.Chi2() > 1) isFitted = false;
    auto fitPars = r.Parameters();
    for (auto& fi : fitPars) sCurrentFitPoint += fi*TMath::Power(sCurrentPointX, double(&fi - &fitPars[0]));
    if ((1.0/sCurrentPointVal) * (sCurrentPointVal - sCurrentFitPoint) * (sCurrentPointVal - sCurrentFitPoint) >= eps && isFitted) return true;
    return false;
  }
  */
/**
* @brief Compares current value with neighbors...
*        If current point more than c*(value[i-1] + value[i+1])/ 2 likely it's emission.
* @param c - scale factor
* @return true if current point more than c*(value[i-1] + value[i+1])/ 2
*/
  /*
  bool IsMoreThanCNeighbors(double c) {
    double leftValue{}, rightValue{};
    if (sCurrentIndexInWindow > 0)
      leftValue = sWindowArrayVal[sCurrentIndexInWindow - 1];
    else return (sCurrentPointVal > (c*sWindowArrayVal[sCurrentIndexInWindow + 1]));
  
    if (sCurrentIndexInWindow < (sWindowSize - 1)) 
      rightValue = sWindowArrayVal[sCurrentIndexInWindow + 1];
    else return (sCurrentPointVal > (c*sWindowArrayVal[sCurrentIndexInWindow - 1]));

    return (sCurrentPointVal > ((c / 2) * (rightValue + leftValue)));
  }
*/
}

