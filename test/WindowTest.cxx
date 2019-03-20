#include "Window.h"
#include <iostream>
#include "gtest/gtest.h"

TEST(WindowTest, FormingWindow) {
  std::vector<double> data = {0,1,4,9,16,25,36,490,64,91};
  
//   std::vector<double> w0 = {0,1,4,9,16}; 
//   std::vector<double> w1 = {0,1,4,9,16}; 
//   std::vector<double> w2 = {0,1,4,9,16}; 
//   std::vector<double> w3 = {1,4,9,16,25}; 
//   std::vector<double> w4 = {4,9,16,25,36}; 
//   std::vector<double> w5 = {9,16,25,36,490}; 
//   std::vector<double> w6 = {16,25,36,490,64}; 
//   std::vector<double> w7 = {25,36,490,64,91}; 
//   std::vector<double> w8 = {25,36,490,64,91}; 
//   std::vector<double> w9 = {25,36,490,64,91}; 
  
  std::vector<std::vector<double>> windows = { {0,1,4,9,16},   {0,1,4,9,16},     {0,1,4,9,16},      {1,4,9,16,25}, \
                                               {4,9,16,25,36}, {9,16,25,36,490}, {16,25,36,490,64}, {25,36,490,64,91}, \
                                               {25,36,490,64,91}, {25,36,490,64,91} };
  
  auto wR = new SpectrumId::Window(data, 0);                                         
                                               
  for (auto& wi : windows) {
    wR = new SpectrumId::Window(data, &wi - &windows[0]);       
    ASSERT_EQ(5, wR->GetWindowArray().size());
    for (auto& vi : wi)
     ASSERT_EQ(vi, wR->GetWindowArray()[&vi - &wi[0]]);
  }
  
}


