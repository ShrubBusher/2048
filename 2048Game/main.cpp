#include <QApplication>
#include "mainwindow.h"
//#include "glwidget.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  MainWindow window;
  //GLWidget widget;

  window.setWindowIcon(QIcon(":/t/images/2048_logo.ico"));
  window.setWindowTitle("2048");
  window.show();

  return app.exec();
}
