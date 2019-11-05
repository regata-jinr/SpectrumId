#pragma once

#include <vector>
#include <string>

namespace Utilities {
  template <typename T> 
  std::string Vec2String(const std::vector<T>& v);
 // bool IsEmission(Window w, std::string curve, double eps); 
  class Fitter {
    public:
      static std::string curve;
      static double precision;
      static double currentFitPoint;
      static bool IsEmission(std::vector<double> arrY, int currentIndex);
      static bool IsEmission(std::vector<double> arrX, std::vector<double> arrY, int currentIndex);
      static bool IsEmission(std::vector<double> arrX, std::vector<double> arrY, std::vector<double> arrYErr, int currentIndex);
      static bool IsEmission(std::vector<double> arrX, std::vector<double> arrY, std::vector<double> arrXErr, std::vector<double> arrYErr, int currentIndex);
    };
  }

