#include "Utilities.h"
#include "gtest/gtest.h"

TEST(UtilitiesTest, EmissionsCount) {
  std::vector<double> data = {0,1,4,9,16,25,36,490,64,91};
  int emissionsCnt{};

  for (const auto& di : data) {
    if (Utilities::Fitter::IsEmission(data, &di - &data[0])) 
      emissionsCnt++;
  }
   
  ASSERT_EQ(1, emissionsCnt);
}

TEST(UtilitiesTest, FittingValue) {
  std::vector<double> data = {0,1,4,9,16,25,36,490,64,91};
  int emissionsCnt{};
  double fitVal{};
  for (const auto& di : data) {
    if (Utilities::Fitter::IsEmission(data, &di - &data[0])) {
      emissionsCnt++;
      fitVal = Utilities::Fitter::currentFitPoint;
    }
  }

  std::cout << "UtilitiesTest::FittingValue is " << fitVal << std::endl; 
  ASSERT_NEAR(49.0, fitVal, 5.0);
  
}