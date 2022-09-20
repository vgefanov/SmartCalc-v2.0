#include <gtest/gtest.h>

#include "model.h"

TEST(SmartCalc2, test1) {
  s21::CalcModel model;
  std::string expresion = "cos(1)";
  std::string result = model.CalculateCalc(expresion, 0);
  ASSERT_EQ("0.540302", result);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
