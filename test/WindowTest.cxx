#include "Window.h"
#include "gtest/gtest.h"

TEST(WindowTest, Indexes) { 
  
  std::vector<double> data = {0,1,4,9,16,25,36,490,64,91};
  int emissionsCnt{};
  auto wR = new SpectrumId::Window(data,0);
  for (const auto& di : data) {
    wR = new SpectrumId::Window(data, &di - &data[0]);
    ASSERT_EQ(0, wR->GetStartIndex());
    ASSERT_EQ(5, wR->GetEndIndex());
  }
  
}

