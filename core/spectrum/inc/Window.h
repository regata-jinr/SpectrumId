#pragma once

 /* Copyright(c) 2018-2019, REGATA Experiment at JINR, All rights reserved. *
  * See cxx source for full Copyright notice                           */

 /*!
  * \brief Window is a moving area which gets subset of spectrum data.
  *
  * \author <a href="mailto:bdrum@jinr.ru">Boris Rumyantsev</a>
 */

#include <vector>
#include <string>

namespace SpectrumId {
class Window {
  private:
    std::vector<double> sxData;
    std::vector<double> sxErr;
    std::vector<double> svData;
    std::vector<double> svErr;
    
    std::vector<double> sWindowArrayX;
    std::vector<double> sWindowArrayXExceptCurrentPoint;
    
    std::vector<double> sWindowArrayXErr;
    std::vector<double> sWindowArrayXErrExceptCurrentPoint;
    
    std::vector<double> sWindowArrayVal;
    std::vector<double> sWindowArrayValExceptCurrentPoint;
    
    std::vector<double> sWindowArrayValErr;
    std::vector<double> sWindowArrayValErrExceptCurrentPoint;
    
    double sCurrentPointVal;
    double sCurrentPointX;
    double sCurrentPointErrX;
    double sCurrentFitPoint;
    double sCurrentPointErrVal;
    int sCurrentIndex;
    int sWindowSize;
    int sCurrentIndexInWindow;
    
    void FormArrays(std::vector<double>& origArray, std::vector<double>& newArray, std::vector<double>& modifiedArray);
    
  public:
    Window (const std::vector<double>& val, int currentIndex, int windowSize=5);
    Window (const std::vector<double>& x, const std::vector<double>& val, int currentIndex, int windowSize = 5 );
    Window (const std::vector<double>& x, const std::vector<double>& val, const std::vector<double>& xErr, const std::vector<double>& valErr, int currentIndex, int windowSize=5);
    ~Window(){};
    
    bool IsCurrentValueEmission(std::string curve="pol2", double eps=0.15);
    bool IsMoreThanCNeighbors(double c);
    double GetFitPointValue() {return sCurrentFitPoint;};
    
  };
}
