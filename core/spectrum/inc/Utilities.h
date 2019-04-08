#pragma once

#include <vector>
#include <string>

namespace Utilities {
  template <typename T> 
  std::string Vec2String(const std::vector<T>& v);

  template <typename T> 
  std::string Vec2String(const std::vector<T>& v) {
    std::string s = "";
  
    for (const auto & vi: v) {
      if (vi != v.back()) s += std::to_string(vi) + ", ";
      else s += std::to_string(vi);
    }
    return s;
  }
}
