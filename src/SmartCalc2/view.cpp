#include "view.h"

#include <QRegularExpression>

#include "ui_view.h"

namespace s21 {
View::View(s21::Controller *controller, QWidget *parent)
    : QMainWindow(parent), controller(controller), ui(new Ui::View) {
  ui->setupUi(this);
  scene = new QGraphicsScene(this);
  setFixedSize(this->width() - 508, this->height());
  ui->lineEdit_value_x->hide();
  ui->label->hide();
  scene->setSceneRect(0, 0, ui->graphicsView->width() - 10,
                      ui->graphicsView->height() - 10);
  scene->addLine(0, scene->height() / 2, scene->width(), scene->height() / 2);
  scene->addLine(scene->width() / 2, 0, scene->width() / 2, scene->height());
  DrawNet(ui->lineEdit_x_end->text().isEmpty()
              ? 100
              : ui->lineEdit_x_end->text().toDouble(),
          ui->lineEdit_y_end->text().isEmpty()
              ? 100
              : ui->lineEdit_y_end->text().toDouble());
  ui->graphicsView->setScene(scene);
}

void View::connectSignals() {
  for (int i = 0; i < ui->gridLayout_2->count(); i++) {
    QPushButton *button =
        qobject_cast<QPushButton *>(ui->gridLayout_2->itemAt(i)->widget());
    if (button->text() == "=") {
      connect(button, SIGNAL(clicked()), this, SLOT(calculateResult()));
    } else if (button->text() == "AC") {
      connect(button, SIGNAL(clicked()), this, SLOT(pushButton_AC_clicked()));
    } else if (button->text() == "Backspace") {
      connect(button, SIGNAL(clicked()), this, SLOT(backspaceButtonClicked()));
    } else if (button->text() == "More") {
      connect(button, SIGNAL(clicked()), this, SLOT(moreButtonClicked()));
    } else if (button->text() == ".") {
      connect(button, SIGNAL(clicked()), this, SLOT(checkDot()));
    } else if ((button->text() == "*") || (button->text() == "/") ||
               (button->text() == "%") || (button->text() == "^")) {
      connect(button, SIGNAL(clicked()), this, SLOT(from_button_operations()));
    } else if (button->text() == "(") {
      connect(button, SIGNAL(clicked()), this, SLOT(checkLBrackets()));
    } else if (button->text() == ")") {
      connect(button, SIGNAL(clicked()), this, SLOT(checkRBrackets()));
    } else if ((button->text() == "+") || (button->text() == "-")) {
      connect(button, SIGNAL(clicked()), this,
              SLOT(from_button_operations_unary()));
    } else if ((button->text() == "sin") || (button->text() == "cos") ||
               (button->text() == "tan") || (button->text() == "asin") ||
               (button->text() == "acos") || (button->text() == "atan") ||
               (button->text() == "ln") || (button->text() == "log") ||
               (button->text() == "sqrt")) {
      connect(button, SIGNAL(clicked()), this, SLOT(checkFuncs()));
    } else {
      connect(button, SIGNAL(clicked()), this, SLOT(editButtonClicked()));
    }
  }
  connect(ui->lineEdit, SIGNAL(textChanged(QString)), this, SLOT(checkX()));
  connect(ui->pushButton_graf, SIGNAL(clicked()), this,
          SLOT(drawButtonClicked()));
  connect(ui->pushButton_raschet, SIGNAL(clicked()), this,
          SLOT(CalcCreditButtonClicked()));
  connect(ui->pushButton_clear_2, SIGNAL(clicked()), this,
          SLOT(pushButton_AC_clicked()));
}

void View::checkX() {
  if (ui->lineEdit->text().contains("x")) {
    ui->lineEdit_value_x->show();
    ui->label->show();
  } else {
    ui->lineEdit_value_x->hide();
    ui->label->hide();
  }
}

QRegularExpressionMatch View::match(QString check_string, QString rx_string) {
  QRegularExpression rx(rx_string);
  QRegularExpressionMatch match = rx.match(check_string);
  return match;
}

bool View::check_and_set_text(QString new_label, QString rx_string,
                              bool is_empty_check) {
  bool result = 0;
  bool first_check = (is_empty_check) ? (new_label == "") : 0;
  if (first_check || match(new_label, rx_string).hasMatch()) {
    QPushButton *button = (QPushButton *)sender();
    new_label = new_label + button->text();
    ui->lineEdit->setText(new_label);
    result = 1;
  }
  return result;
}

void View::checkDot() {
  QString new_label = ui->lineEdit->text();
  QString capture = match(new_label, "[\\.|\\d]+$").captured();
  if (capture != "" && !capture.contains(".")) {
    ui->lineEdit->setText(new_label + ".");
  }
}

int View::countBrackets(QString label) {
  int lb = 0, rb = 0;
  for (QString::iterator itr(label.begin()); itr != label.end(); ++itr) {
    if (*itr == '(') ++lb;
    if (*itr == ')') ++rb;
  }
  return (lb - rb);
}

void View::checkLBrackets() {
  QString new_label = ui->lineEdit->text();
  check_and_set_text(new_label, "\\D", true);
}

void View::checkRBrackets() {
  QString new_label = ui->lineEdit->text();
  if (countBrackets(new_label) > 0) {
    check_and_set_text(new_label, "(\\)|\\d|x)$", false);
  }
}

void View::checkDigit() {
  QString new_label = ui->lineEdit->text();
  if (!match(new_label, "[^\\d|\\.]0$").hasMatch()) {
    check_and_set_text(new_label, "(\\(| |\\d|\\.|x|\\+|-|\\/|\\^)$", true);
  }
}

void View::checkFuncs() {
  QString new_label = ui->lineEdit->text();
  if (check_and_set_text(new_label, "\\S", true)) {
    ui->lineEdit->setText(ui->lineEdit->text() + "(");
  }
}

void View::from_button_operations_unary() {
  QString new_label = ui->lineEdit->text();
  check_and_set_text(new_label, "(\\(| |\\)|\\d|x)$", true);
}

void View::from_button_operations() {
  QString new_label = ui->lineEdit->text();
  check_and_set_text(new_label, "(\\)|\\d|x)$", false);
}

void View::moreButtonClicked() {
  if (stat == HIDE) {
    setFixedSize(this->width() + 508, this->height());
    stat = SHOW;
  } else {
    setFixedSize(this->width() - 508, this->height());
    stat = HIDE;
  }
}

void View::on_tabWidget_currentChanged(int index) {
  if (stat == SHOW && (index == 1 || index == 2)) {
    setFixedSize(this->width() - 508, this->height());
    stat = HIDE;
  }
}

void View::pushButton_AC_clicked() {
  ui->lineEdit->clear();
  ui->lineEdit_value_x->clear();
  ui->lineEdit_x_end->clear();
  ui->lineEdit_y_end->clear();
  ui->lineEdit_SummaCredita->clear();
  ui->lineEdit_SrokCredita->clear();
  ui->lineEdit_PercentStavka->clear();
  ui->label_MesPlatez->clear();
  ui->label_Pereplata->clear();
  ui->label_VsegoPlatez->clear();
  scene->clear();
  DrawNet(ui->lineEdit_x_end->text().isEmpty()
              ? 100
              : ui->lineEdit_x_end->text().toDouble(),
          ui->lineEdit_y_end->text().isEmpty()
              ? 100
              : ui->lineEdit_y_end->text().toDouble());
}

void View::backspaceButtonClicked() { ui->lineEdit->backspace(); }

void View::editButtonClicked() {
  QString expresion = ui->lineEdit->displayText();
  QPushButton *buttonSender = qobject_cast<QPushButton *>(sender());
  expresion += buttonSender->text();
  ui->lineEdit->setText(expresion);
}

void View::calculateResult() {
  std::string str = ui->lineEdit->displayText().toStdString();
  double x = ui->lineEdit_value_x->text().toDouble();
  QString result(controller->calc(str, x).c_str());
  ui->lineEdit->setText(result);
}

void View::DrawNet(double x_max, double y_max) {
  QGraphicsTextItem *X, *Y;
  double heightStep = scene->height() / 6;
  double widthStep = scene->width() / 6;
  int step = -3;
  for (int i = 0; i <= 6; i++) {
    QGraphicsTextItem *num1, *num2;
    num1 = scene->addText(QString::number((x_max / 6) * step, 'g', 2));
    num1->setPos(widthStep * i, scene->height() / 2);
    num1->setDefaultTextColor(Qt::red);
    num2 = scene->addText(QString::number((y_max / 6) * step, 'g', 2));
    num2->setPos(scene->width() / 2, heightStep * (6 - i));
    num2->setDefaultTextColor(Qt::red);
    step++;
  }
  X = scene->addText("X");
  X->setPos(scene->width() - 18, scene->height() / 2);
  X->setDefaultTextColor(Qt::red);
  Y = scene->addText("Y");
  Y->setPos(scene->width() / 2 - 18, 0);
  Y->setDefaultTextColor(Qt::red);
  scene->addLine(0, scene->height() / 2, scene->width(), scene->height() / 2);
  scene->addLine(scene->width() / 2, 0, scene->width() / 2, scene->height());
}

void View::drawButtonClicked() {
  QPen red(Qt::red);
  scene->clear();
  double x_max = ui->lineEdit_x_end->text().isEmpty()
                     ? 100
                     : ui->lineEdit_x_end->text().toDouble() * 2;
  double y_max = ui->lineEdit_y_end->text().isEmpty()
                     ? 100
                     : ui->lineEdit_y_end->text().toDouble() * 2;
  DrawNet(x_max, y_max);
  std::string str = ui->lineEdit->displayText().toStdString();
  std::vector<std::pair<double, double>> coord =
      controller->getCoordinates(str, x_max);
  for (int i = 1; i < 10000; i++) {
    double x_prev, y_prev, x_curr, y_curr;
    x_prev = coord[i - 1].first * scene->width() / x_max + scene->width() / 2;
    y_prev =
        -coord[i - 1].second * scene->height() / y_max + scene->height() / 2;
    x_curr = coord[i].first * scene->width() / x_max + scene->width() / 2;
    y_curr = -coord[i].second * scene->height() / y_max + scene->height() / 2;
    scene->addLine(x_prev, y_prev, x_curr, y_curr, red);
  }
}

void View::CalcCreditButtonClicked() {
  double SummaCredita = ui->lineEdit_SummaCredita->text().toDouble();
  int SrokCredita;
  if (ui->comboBox_srok->currentIndex() == 0) {
    SrokCredita = ui->lineEdit_SrokCredita->text().toInt();
  } else {
    SrokCredita = ui->lineEdit_SrokCredita->text().toInt() * 12;
  }
  double PercentStavka = ui->lineEdit_PercentStavka->text().toDouble();
  auto credit_type =
      ui->comboBox_tip->currentIndex() ? DIFFERENTIATED : ANNUITY;
  auto result = controller->CalcCredit(SummaCredita, SrokCredita, PercentStavka,
                                       credit_type);
  if (credit_type == DIFFERENTIATED) {
    ui->label_MesPlatez->setText(QString::number(result.MesPlatez1, 'f', 2) +
                                 " ... " +
                                 QString::number(result.MesPlatez, 'f', 2));
  } else {
    ui->label_MesPlatez->setText(QString::number(result.MesPlatez, 'f', 2));
  }
  ui->label_Pereplata->setText(QString::number(result.Pereplata, 'f', 2));
  ui->label_VsegoPlatez->setText(QString::number(result.VsegoPlatez, 'f', 2));
}

View::~View() { delete ui; }

}  // namespace s21
