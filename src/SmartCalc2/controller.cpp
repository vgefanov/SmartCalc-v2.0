#include "controller.h"

std::string s21::Controller::calc(std::string const &expresion, double x) {
  return model->CalculateCalc(expresion, x);
}

std::vector<std::pair<double, double>> s21::Controller::getCoordinates(
    std::string const &expresion, double x_max) {
  return model->calcCoordinates(expresion, x_max);
}

s21::credit s21::Controller::CalcCredit(double SummaCredita, int SrokCredita,
                                        double PercentStavka,
                                        credit_type type) {
  return model->CreditCalc(SummaCredita, SrokCredita, PercentStavka, type);
}
