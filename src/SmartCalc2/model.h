#ifndef SRC_SMARTCALC2_MODEL_H_
#define SRC_SMARTCALC2_MODEL_H_

#include <cmath>
#include <iostream>
#include <regex>
#include <stack>
#include <string>
#include <vector>

namespace s21 {

class PNModel {
  std::string expresion;
  std::stack<std::string> operation;
  std::vector<std::string> notation;

 public:
  PNModel() {}
  PNModel(const PNModel &other) = delete;
  PNModel(PNModel &&other) = delete;
  PNModel &operator=(const PNModel &other) = delete;
  PNModel &operator=(PNModel &&other) = delete;
  explicit PNModel(std::string);
  ~PNModel() {}
  double CalculateExpressions(double x = 0);
  void SetExpressions(std::string);

 private:
  void TranslateExpressions();
  bool CheckRegex();
  void DigitOperations(int &index);
  int PriorityOperations(std::string const &oper);
  void OperatorOperations(int &index);
  int ClosingBracketOperations();
  void FunctionOperations(int &index);
  void PushOperations();
  void ClearData();
  bool StringIsNumber(std::string);
  void FunctionAction(std::stack<double> &number, std::string action);
  void OperatorAction(std::stack<double> &number, std::string action);
  void ChooseAction(std::stack<double> &number, std::string action);
};

enum credit_type { ANNUITY, DIFFERENTIATED };

typedef struct credit {
  double SummaCredita;
  int SrokCredita;
  double PercentStavka;
  credit_type type;
  double MesPlatez;
  double MesPlatez1;
  double Pereplata;
  double VsegoPlatez;
} credit_struct;

class Credit {
 public:
  Credit() {
    data.SummaCredita = 0;
    data.SrokCredita = 0;
    data.PercentStavka = 0;
    data.type = ANNUITY;
    data.MesPlatez = 0;
    data.MesPlatez1 = 0;
    data.Pereplata = 0;
    data.VsegoPlatez = 0;
  }
  Credit(const Credit &other) = delete;
  Credit(Credit &&other) = delete;
  Credit &operator=(const Credit &other) = delete;
  Credit &operator=(Credit &&other) = delete;
  ~Credit() {}
  void CalculateCredit();
  void SetCredit(double, int, double, credit_type);
  credit_struct GetData() { return data; }

 private:
  credit_struct data;
};

class CalcModel {
  PNModel pnmodel;
  Credit credit;

 public:
  CalcModel() {}
  CalcModel(const CalcModel &other) = delete;
  CalcModel(CalcModel &&other) = delete;
  CalcModel &operator=(const CalcModel &other) = delete;
  CalcModel &operator=(CalcModel &&other) = delete;
  ~CalcModel() {}
  std::string CalculateCalc(std::string const &expresion, double x);
  std::vector<std::pair<double, double>> calcCoordinates(
      std::string const &expresion, double x_max);
  credit_struct CreditCalc(double, int, double, credit_type);
};

}  // namespace s21

#endif  // SRC_SMARTCALC2_MODEL_H_
