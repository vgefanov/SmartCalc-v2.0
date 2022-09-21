#ifndef SRC_SMARTCALC2_VIEW_H_
#define SRC_SMARTCALC2_VIEW_H_
#include <QDate>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>
#include <QRegularExpression>

#include "controller.h"

enum DetailStatus { SHOW, HIDE };

QT_BEGIN_NAMESPACE
namespace Ui {
class View;
}
QT_END_NAMESPACE
namespace s21 {
class View : public QMainWindow {
  Q_OBJECT
  s21::Controller *controller;

 public:
  explicit View(s21::Controller *controller, QWidget *parent = nullptr);
  ~View();
  void connectSignals();
 private slots:
  void moreButtonClicked();
  void pushButton_AC_clicked();
  void backspaceButtonClicked();
  void editButtonClicked();
  void calculateResult();
  void checkX();
  void drawButtonClicked();
  void checkDot();
  void from_button_operations_unary();
  void from_button_operations();
  void checkLBrackets();
  void checkRBrackets();
  void checkFuncs();
  void CalcCreditButtonClicked();

  void on_tabWidget_currentChanged(int index);

 private:
  QRegularExpressionMatch match(QString string, QString rx);
  bool check_and_set_text(QString new_label, QString rx_string,
                          bool is_empty_check);
  int countBrackets(QString label);
  QString get_result_show_string();
  void DrawNet(double x_max, double y_max);
  DetailStatus stat = HIDE;
  Ui::View *ui;
  QGraphicsScene *scene;
  std::multimap<QDate, long double> events;
};
}  // namespace s21

#endif  // SRC_SMARTCALC2_VIEW_H_
