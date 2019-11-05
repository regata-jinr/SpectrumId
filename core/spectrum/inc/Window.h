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
    std::vector<double> wWindowArray;
    
    int wCurrentIndex;
    int wWindowSize;
    int wCurrentWindowIndex;
    int wInputArraySize;
    int wStartIndex;
    int wEndIndex;
    void FormIndexes();
    
  public:
    Window (const std::vector<double>& val, int currentIndex, int windowSize = 5);
    int GetStartIndex() {return wStartIndex;} 
    int GetEndIndex() {return wEndIndex;} 
    int GetIndexInWindow() {return wCurrentWindowIndex;} 
    std::vector<double> GetWindowArray() {return wWindowArray;} 
    
  };
}
