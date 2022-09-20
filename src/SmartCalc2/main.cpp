#include <QApplication>

#include "controller.h"
#include "model.h"
#include "view.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  s21::CalcModel model;
  s21::Controller controller(&model);
  s21::View w(&controller);
  w.connectSignals();
  w.show();
  return a.exec();
}
