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
#include "Utilities.h"


void SpectrumId::Window::FormIndexes() {
    wStartIndex = wCurrentIndex - wCurrentWindowIndex;
    wEndIndex = wCurrentIndex + wCurrentWindowIndex;
   

    //boundary points
    if (wStartIndex <= 0) {
      wCurrentWindowIndex += wStartIndex;
      wStartIndex = 0;
      wEndIndex = wWindowSize - 1;
    }

    if (wEndIndex >= wInputArraySize) {
      wStartIndex = wInputArraySize - wWindowSize;
      wEndIndex = wInputArraySize - 1;
      wCurrentWindowIndex = wWindowSize - (wInputArraySize - wCurrentIndex - 1) - 1;
    }
    //boundary points  
    ::Info("Window::FormArrays", "wStartIndex - %d, endIndex - %d", wStartIndex, wEndIndex); 
}


/**
* @brief Constructor of window
* Window is subset of input array with specified size.
* @param arr - input vector
* @param currentIndex - which value from input vector should be a center of window
* @param windowSize - size of window. 5 by default.
*/
SpectrumId::Window::Window(const std::vector<double>& arr, int currentIndex, int windowSize) : wCurrentIndex(currentIndex),\
                                                                                               wWindowSize(windowSize),\
                                                                                               wCurrentWindowIndex(windowSize / 2),
                                                                                               wInputArraySize(arr.size())
{
    if (arr.empty()) throw std::invalid_argument("Input data for Window is empty");
    if (wCurrentIndex >= wInputArraySize) ::Error("SpectrumId::Window::FormIndexes", "currentIndex more than size of input array");

    if (wWindowSize < 3) {
      ::Warning("Window::Window", "Size of window too small. 3 will be set.");
      wWindowSize = 3;
    }
    
    FormIndexes();
    wWindowArray = std::vector<double>(arr.begin() + wStartIndex, arr.begin() + wEndIndex + 1);
    }

