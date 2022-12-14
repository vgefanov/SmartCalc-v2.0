#ifndef SRC_SMARTCALC2_CONTROLLER_H_
#define SRC_SMARTCALC2_CONTROLLER_H_

#include <iostream>
#include <string>

#include "model.h"

namespace s21 {
class Controller {
  CalcModel *model;

 public:
  Controller() : model(nullptr) {}
  Controller(const Controller &other) = delete;
  Controller(Controller &&other) = delete;
  Controller &operator=(const Controller &other) = delete;
  Controller &operator=(Controller &&other) = delete;
  explicit Controller(CalcModel *model) : model(model) {}
  ~Controller() {}
  std::string calc(std::string const &expresion, double x = 0.0);
  std::vector<std::pair<double, double>> GetCoordinates(
      std::string const &expresion, double x_max);
  credit CalcCredit(double sum, int month, double percent, credit_type type);
};
}  // namespace s21
#endif  // SRC_SMARTCALC2_CONTROLLER_H_
