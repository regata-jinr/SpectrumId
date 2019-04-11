#include "Utilities.h"
#include "gtest/gtest.h"


TEST(UtilitiesTest, Emissions) {
  std::vector<double> data = {0,1,4,9,16,25,36,490,64,91};
  int emissionsCnt{};

  for (const auto& di : data) {
    if (Utilities::Fitter::IsEmission(data, &di - &data[0])) 
      emissionsCnt++;
  }
   
  ASSERT_EQ(1, emissionsCnt);
  ASSERT_NEAR(49.0, Utilities::Fitter::currentFitPoint, 0.3);
  
}
 
