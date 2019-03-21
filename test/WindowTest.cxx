#include "Window.h"
#include "gtest/gtest.h"

// TEST(WindowTest, FormingWindow) {
//   std::vector<double> data = {0,1,4,9,16,25,36,490,64,91};
//  
//   std::vector<std::vector<double>> windows = { {0,1,4,9,16},  {0,1,4,9,16},  {0,1,4,9,16}, \
//                                                {1,4,9,16,25}, {4,9,16,25,36}, {9,16,25,36,490}, \
//                                                {16,25,36,490,64}, {25,36,490,64,91}, {25,36,490,64,91}, \
//                                                {25,36,490,64,91} \
//                                              };
//   
//   auto wR = new SpectrumId::Window(data, 0);                                         
//   
//   
//   
//   
//   for (auto& wi : windows) {
//     wR = new SpectrumId::Window(data, &wi - &windows[0]);     
//     ASSERT_EQ(5, wR->GetWindowArray().size());
//     for (auto& vi : wi)
//      ASSERT_EQ(vi, wR->GetWindowArray()[&vi - &wi[0]]);
//   }
//   
// }

TEST(WindowTest, IsCurrentValueEmission) { 
  std::vector<double> data = {0,1,4,9,16,25,36,490,64,91};
  int emissionsCnt{};
  auto wR = new SpectrumId::Window(data, 0);
  for (const auto& di : data) {
    wR = new SpectrumId::Window(data, &di - &data[0]);
    if (wR->IsCurrentValueEmission()) emissionsCnt++;
  }
  
  ASSERT_EQ(2, emissionsCnt);
  
}
