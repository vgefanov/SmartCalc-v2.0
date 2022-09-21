#include "model.h"

s21::PNModel::PNModel(const std::string expresion) : expresion(expresion) {
  TranslateExpressions();
}

void s21::PNModel::TranslateExpressions() {
  bool check = 1;
  if (CheckRegex()) check = 0;
  for (int i = 0; i < static_cast<int>(expresion.size()) && check; i++) {
    if (isdigit(expresion[i])) {
      DigitOperations(i);
    } else if (expresion[i] == '(') {
      operation.push("(");
    } else if (PriorityOperations(expresion.substr(i, 1)) &&
               expresion[i] != '(') {
      OperatorOperations(i);
    } else if (expresion[i] == ')') {
      check = ClosingBracketOperations() ? 0 : 1;
    } else if (expresion[i] == 'x') {
      notation.push_back("x");
    } else if ((PriorityOperations(expresion.substr(i, 1)) == 4 ||
                PriorityOperations(expresion.substr(i, 4)) == 1)) {
      FunctionOperations(i);
    }
    if (i == -1) check = 0;
  }
  if (check)
    PushOperations();
  else
    ClearData();
}

bool s21::PNModel::CheckRegex() {
  bool result = 0;
  std::regex regular(
      R"(^\.|\.$|[0-9][a-zA-Z]|[a-zA-Z][0-9]|[0-9]\.[^0-9]|[0-9]\.[0-9]+\.)");
  std::smatch match;
  result = std::regex_search(expresion, match, regular);
  return result;
}

void s21::PNModel::DigitOperations(int &index) {
  int length = 0;
  while (isdigit(expresion[index + length]) || expresion[index + length] == '.')
    length++;
  std::string number = expresion.substr(index, length);
  notation.push_back(number);
  index += length - 1;
}

void s21::PNModel::OperatorOperations(int &index) {
  int priority = PriorityOperations(expresion.substr(index, 1));
  if (priority == 4 && !isdigit(expresion[index - 1]) &&
      expresion[index - 1] != ')' && expresion[index - 1] != 'x') {
    FunctionOperations(index);
  } else {
    while (!operation.empty()) {
      int priority2 = PriorityOperations(operation.top());
      if (priority2 <= priority) {
        notation.push_back(operation.top());
        operation.pop();
      } else {
        break;
      }
    }
    operation.push(expresion.substr(index, 1));
  }
}

int s21::PNModel::ClosingBracketOperations() {
  int error = 0;
  while (!operation.empty() && operation.top() != "(") {
    notation.push_back(operation.top());
    operation.pop();
  }
  if (!operation.empty())
    operation.pop();
  else
    error = 1;
  return error;
}

void s21::PNModel::FunctionOperations(int &index) {
  if (expresion.substr(index, 3) == "cos") {
    operation.push("cos");
    index += 2;
  } else if (expresion.substr(index, 3) == "sin") {
    operation.push("sin");
    index += 2;
  } else if (expresion.substr(index, 3) == "tan") {
    operation.push("tan");
    index += 2;
  } else if (expresion.substr(index, 4) == "acos") {
    operation.push("acos");
    index += 3;
  } else if (expresion.substr(index, 4) == "asin") {
    operation.push("asin");
    index += 3;
  } else if (expresion.substr(index, 4) == "atan") {
    operation.push("atan");
    index += 3;
  } else if (expresion.substr(index, 4) == "sqrt") {
    operation.push("sqrt");
    index += 3;
  } else if (expresion.substr(index, 2) == "ln") {
    operation.push("ln");
    index += 1;
  } else if (expresion.substr(index, 3) == "log") {
    operation.push("log");
    index += 2;
  } else if (expresion.substr(index, 1) == "-") {
    operation.push("unary_minus");
  } else if (expresion.substr(index, 1) == "+") {
    operation.push("unary_plus");
  } else {
    index = -1;
  }
}

void s21::PNModel::PushOperations() {
  while (!operation.empty()) {
    if (operation.top() == "(") {
      ClearData();
    } else {
      notation.push_back(operation.top());
      operation.pop();
    }
  }
}

int s21::PNModel::PriorityOperations(std::string const &operation) {
  int priority = 0;
  if (operation.size() > 1)
    priority = 1;
  else if (operation == "^")
    priority = 2;
  else if (operation == "*" || operation == "/" || operation == "%")
    priority = 3;
  else if (operation == "+" || operation == "-")
    priority = 4;
  else if (operation == "(")
    priority = 5;
  return priority;
}

bool s21::PNModel::StringIsNumber(std::string str) {
  bool result = 1;
  if ((str[0] == '-' && str.size() > 1) || isdigit(str[0])) {
    for (int i = 1; i < static_cast<int>(str.size()) && result; i++) {
      if (!isdigit(str[i]) && str[i] != '.') result = 0;
    }
  } else {
    result = 0;
  }
  return result;
}

void s21::PNModel::FunctionAction(std::stack<double> &number,
                                  const std::string action) {
  double num = number.top();
  number.pop();
  if (action == "cos")
    number.push(cos(num));
  else if (action == "sin")
    number.push(sin(num));
  else if (action == "acos")
    number.push(acos(num));
  else if (action == "asin")
    number.push(asin(num));
  else if (action == "tan")
    number.push(tan(num));
  else if (action == "atan")
    number.push(atan(num));
  else if (action == "log")
    number.push(log10(num));
  else if (action == "ln")
    number.push(log(num));
  else if (action == "sqrt")
    number.push(sqrt(num));
  else if (action == "unary_minus")
    number.push(-num);
  else if (action == "unary_plus")
    number.push(num);
}

void s21::PNModel::OperatorAction(std::stack<double> &number,
                                  const std::string action) {
  double second = number.top();
  number.pop();
  double first = number.top();
  number.pop();
  if (action == "+")
    number.push(first + second);
  else if (action == "-")
    number.push(first - second);
  else if (action == "*")
    number.push(first * second);
  else if (action == "/")
    number.push(first / second);
  else if (action == "%")
    number.push(fmod(first, second));
  else if (action == "^")
    number.push(powl(first, second));
}

void s21::PNModel::ChooseAction(std::stack<double> &number,
                                const std::string action) {
  if (PriorityOperations(action) > 1) {
    OperatorAction(number, action);
  } else {
    FunctionAction(number, action);
  }
}

double s21::PNModel::CalculateExpressions(double x) {
  std::stack<double> numbers;
  double result = NAN;
  if (notation.size() != 0) {
    for (auto iter = notation.begin(); iter < notation.end(); iter++) {
      if (StringIsNumber(*iter)) {
        numbers.push(std::stod(*iter));
      } else if (*iter == "x") {
        numbers.push(x);
      } else {
        ChooseAction(numbers, *iter);
      }
    }
    result = numbers.top();
    numbers.pop();
  }
  return result;
}

void s21::PNModel::SetExpressions(const std::string expresion) {
  this->expresion = expresion;
  ClearData();
  TranslateExpressions();
}

void s21::PNModel::ClearData() {
  while (operation.size() != 0) operation.pop();
  notation.clear();
}

std::string s21::CalcModel::CalculateCalc(std::string const &expresion,
                                          double x) {
  pnmodel.SetExpressions(expresion);
  double num = pnmodel.CalculateExpressions(x);
  std::string result = std::to_string(num);
  return result;
}

std::vector<std::pair<double, double>> s21::CalcModel::calcCoordinates(
    std::string const &expresion, double x_max) {
  pnmodel.SetExpressions(expresion);
  double x = -x_max;
  double step = (x_max * 2) / 10000;
  std::vector<std::pair<double, double>> coord(10000);
  for (int i = 0; i < 10000; i++) {
    coord[i].first = x;
    coord[i].second = pnmodel.CalculateExpressions(x);
    x += step;
  }
  return coord;
}

void s21::Credit::CalculateCredit() {
  double MesPercentStavka = data.PercentStavka / 12 / 100;
  if (data.type == ANNUITY) {
    double AnnuityCof =
        (MesPercentStavka * pow((1 + MesPercentStavka), data.SrokCredita)) /
        (pow((1 + MesPercentStavka), data.SrokCredita) - 1);
    data.MesPlatez = data.SummaCredita * AnnuityCof;
    data.VsegoPlatez = data.MesPlatez * data.SrokCredita;
  } else {
    for (int i = 0; i < data.SrokCredita; i++) {
      double ChastOsnDolga = data.SummaCredita / data.SrokCredita;
      data.MesPlatez =
          ChastOsnDolga +
          ((data.SummaCredita - ChastOsnDolga * i) * MesPercentStavka);
      if (i == 0) {
        data.MesPlatez1 = data.MesPlatez;
      }
      data.VsegoPlatez += data.MesPlatez;
    }
  }
  data.Pereplata = data.VsegoPlatez - data.SummaCredita;
}

void s21::Credit::SetCredit(double SummaCredita, int SrokCredita,
                            double PercentStavka, credit_type type) {
  data.SummaCredita = SummaCredita;
  data.SrokCredita = SrokCredita;
  data.PercentStavka = PercentStavka;
  data.type = type;
  data.MesPlatez = 0;
  data.MesPlatez1 = 0;
  data.Pereplata = 0;
  data.VsegoPlatez = 0;
  CalculateCredit();
}

s21::credit_struct s21::CalcModel::CreditCalc(double SummaCredita,
                                              int SrokCredita,
                                              double PercentStavka,
                                              credit_type type) {
  credit.SetCredit(SummaCredita, SrokCredita, PercentStavka, type);
  return credit.GetData();
}
