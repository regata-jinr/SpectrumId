#include "Window.h"
#include "gtest/gtest.h"

TEST(WindowTest, IsCurrentValueEmission) { 
  
  std::vector<double> data = {0,1,4,9,16,25,36,490,64,91};
  int emissionsCnt{};
  auto wR = new SpectrumId::Window(data, 0);
  for (const auto& di : data) {
    wR = new SpectrumId::Window(data, &di - &data[0]);
    if (wR->IsCurrentValueEmission()) emissionsCnt++;
  }
  
  ASSERT_EQ(3, emissionsCnt);
  
}

TEST(WindowTest, IsMoreThanCNeighbors) {
  
  std::vector<double> data = {0,1,2,3,4,5,6,7,8,9};
  
  auto wR = new SpectrumId::Window(data, 0);
  ASSERT_EQ(false, wR->IsMoreThanCNeighbors(1));
  ASSERT_EQ(false, wR->IsMoreThanCNeighbors(0));
  
  wR = new SpectrumId::Window(data, 9);
  ASSERT_EQ(true,  wR->IsMoreThanCNeighbors(1));
  ASSERT_EQ(false, wR->IsMoreThanCNeighbors(2));
  
  wR = new SpectrumId::Window(data, 1);
  ASSERT_EQ(false, wR->IsMoreThanCNeighbors(1));
  ASSERT_EQ(false, wR->IsMoreThanCNeighbors(2));
  ASSERT_EQ(true,  wR->IsMoreThanCNeighbors(0.3));
  
}
