#include "Utilities.h"
#include "TF1.h"
#include "Fit/BinData.h"
#include "Fit/Fitter.h"
#include "Math/WrappedMultiTF1.h"
#include "TMath.h"
#include <numeric>
#include <algorithm>

/*
 * @brief Utilities namespace is temporary storage for methods or objects which don't built in some basics classes yet. 
 */
  template <typename T> 
  std::string Utilities::Vec2String(const std::vector<T>& v) {
    std::string s = "";
  
    for (const auto & vi: v) {
      if (vi != v.back()) s += std::to_string(vi) + ", ";
      else s += std::to_string(vi);
    }
    return s;
  }
// @param curve - name of TF1 function which fit window array without current point.
// @param eps - value for comparison with chi2 of current point and fitVal of current point

/*
* @brief Checks current point on emission
* The idea of this checking that emission breaks continuity of curve describing spectrum. 
* @return true if chi2 of current point and fit point more than eps and window could be fit of curve, so current point is emission
*/
  std::string Utilities::Fitter::curve = "pol2";
  double Utilities::Fitter::precision = 0.15;
  double Utilities::Fitter::currentFitPoint{};
  
  bool Utilities::Fitter::IsEmission(std::vector<double> arrX, std::vector<double> arrY, std::vector<double> arrXErr, std::vector<double> arrYErr, int currentIndex) {

    std::vector<int> sizes{ (int) arrX.size(), (int) arrY.size(), (int) arrXErr.size(), (int) arrYErr.size() };

    if (!std::all_of(sizes.begin(), sizes.end(), [&] (int i) { return (i == arrX.size()); })) {
      ::Error("Utilities::IsEmission", "Sizes of input vectos are different.");
      return false;
    }
    if (currentIndex >= arrY.size()) {
      ::Error("Utilities::IsEmission", "Current index more than size of input vectors.");
      return false;
    }
    Utilities::Fitter::currentFitPoint = 0.; 
    double currentXValue = arrX[currentIndex];
    double currentYValue = arrY[currentIndex];

    arrX.erase(arrX.begin() + currentIndex);
    arrY.erase(arrY.begin() + currentIndex);
    arrXErr.erase(arrXErr.begin() + currentIndex);
    arrYErr.erase(arrYErr.begin() + currentIndex);

    std::cout << "Array for fitting: " << Utilities::Vec2String(arrY) << std::endl;

    ROOT::Fit::BinData bData(arrY.size(), arrX.data(),\
                             arrY.data(), arrXErr.data(),\
                             arrYErr.data());
    
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

    for (auto& fi : fitPars) Fitter::currentFitPoint += fi*TMath::Power(currentXValue, double(&fi - &fitPars[0]));
    double pointChi2 = (1.0/currentYValue) * (currentYValue - Fitter::currentFitPoint) * (currentYValue - Fitter::currentFitPoint);

    std::cout << "Chi2 val and fit val: " << pointChi2 << std::endl;

    if ((pointChi2 >= Fitter::precision) && isFitted) return true;
    return false;
  }
  bool Utilities::Fitter::IsEmission(std::vector<double> arrY, int currentIndex) {
    std::vector<double> arrX(arrY.size(), 0);
    std::iota(arrX.begin(), arrX.end(), 0);
    return Fitter::IsEmission(arrX, arrY, currentIndex);
  }
  bool Utilities::Fitter::IsEmission(std::vector<double> arrX, std::vector<double> arrY, int currentIndex) {
    std::vector<double> arrYErr(arrY.size(), 0);
    for (auto i = 0; i < arrY.size(); ++i) {
      arrYErr[i] = TMath::Sqrt(arrY[i]);
     // if (arrY[i] != 0) arrYErr[i] = 1.0 / arrY[i];
     // else arrYErr[i] = 0;
    }

    return Fitter::IsEmission(arrX, arrY, arrYErr, currentIndex);
  }
  
  bool Utilities::Fitter::IsEmission(std::vector<double> arrX, std::vector<double> arrY, std::vector<double> arrYErr, int currentIndex) {
    std::vector<double> arrXErr(arrY.size(), 0);
    return Fitter::IsEmission(arrX, arrY, arrXErr, arrYErr, currentIndex);
  }

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
