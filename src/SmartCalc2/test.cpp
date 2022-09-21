#include <gtest/gtest.h>

#include "model.h"

TEST(SmartCalc2, test1) {
  s21::CalcModel model;
  std::string expresion = "1+2";
  std::string result = model.CalculateCalc(expresion, 0);
  ASSERT_EQ("3.000000", result);
  expresion = "-5";
  result = model.CalculateCalc(expresion, 0);
  ASSERT_EQ("-5.000000", result);
  expresion = "+8";
  result = model.CalculateCalc(expresion, 0);
  ASSERT_EQ("8.000000", result);
  expresion = "2+x";
  result = model.CalculateCalc(expresion, 2);
  ASSERT_EQ("4.000000", result);
  expresion = "asd";
  result = model.CalculateCalc(expresion, 0);
  ASSERT_EQ("nan", result);
  expresion = "2+4*5^2-4/2-5%2";
  result = model.CalculateCalc(expresion, 0);
  ASSERT_EQ("99.000000", result);
  expresion = "(sin(1)+cos(4))*tan(1)-acos(1)+asin(1)*atan(1)";
  result = model.CalculateCalc(expresion, 0);
  ASSERT_EQ("1.526224", result);
  expresion = "sqrt(9)+log(10)-ln(10)";
  result = model.CalculateCalc(expresion, 0);
  ASSERT_EQ("1.697415", result);
  expresion = "";
  result = model.CalculateCalc(expresion, 0);
  ASSERT_EQ("nan", result);
}

TEST(SmartCalc2, test2) {
  s21::CalcModel model;
  s21::credit_struct credit = model.CreditCalc(100000, 12, 10, s21::ANNUITY);
  ASSERT_TRUE(fabs(credit.MesPlatez - 8791.59) < 0.1);
  ASSERT_TRUE(fabs(credit.Pereplata - 5499.06) < 0.1);
  ASSERT_TRUE(fabs(credit.VsegoPlatez - 105499.06) < 0.1);
  credit = model.CreditCalc(200000, 24, 15, s21::DIFFERENTIATED);
  ASSERT_TRUE(fabs(credit.MesPlatez1 - 10833.3) < 0.1);
  ASSERT_TRUE(fabs(credit.MesPlatez - 8437.5) < 0.1);
  ASSERT_TRUE(fabs(credit.Pereplata - 31250) < 0.1);
  ASSERT_TRUE(fabs(credit.VsegoPlatez - 231250) < 0.1);
}

TEST(SmartCalc2, test3) {
  s21::CalcModel model;
  std::string expresion = "5";
  std::vector<std::pair<double, double>> result =
      model.calcCoordinates(expresion, 10);
  ASSERT_EQ(-10, result[0].first);
  ASSERT_EQ(5, result[0].second);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
