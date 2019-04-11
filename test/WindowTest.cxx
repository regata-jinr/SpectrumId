#include "Window.h"
#include "gtest/gtest.h"

TEST(WindowTest, Indexes) { 
  
  std::vector<double> data = {0,1,4,9,16,25,36,49,64};
  std::vector<double> startIndexesArray = {0,0,0,1,2,3,4,4,4};
  std::vector<double> endIndexesArray   = {4,4,4,5,6,7,8,8,8};

  auto wR = new SpectrumId::Window(data,0);
  for (const auto& di : data) {
    wR = new SpectrumId::Window(data, &di - &data[0]);
    ASSERT_EQ(startIndexesArray[&di - &data[0]], wR->GetStartIndex());
    ASSERT_EQ(endIndexesArray[&di - &data[0]], wR->GetEndIndex());
  }
  
}

