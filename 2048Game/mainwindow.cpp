#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "glwidget.h"
#include <iostream>
#include <string>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
  GLWidget *widget = new GLWidget;
  QObject::connect(this, &MainWindow::SignalKeyW, widget, &GLWidget::MoveUp);
  QObject::connect(this, &MainWindow::SignalKeyA, widget, &GLWidget::MoveLeft);
  QObject::connect(this, &MainWindow::SignalKeyS, widget, &GLWidget::MoveDown);
  QObject::connect(this, &MainWindow::SignalKeyD, widget, &GLWidget::MoveRight);
  ui->setupUi(this);
  ui->winlabel->hide();
}

int MainWindow::window_score = 0;

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
  ui->winlabel->setHidden(!(GLWidget::game_end_changed()));
  switch (event->key())
  {
  case Qt::Key_W:
    std::cout << "keyPressEvent override    -   Key_W" << std::endl;
    emit SlotKeyW();
    break;

  case Qt::Key_A:
    std::cout << "keyPressEvent override    -   Key_A" << std::endl;
    emit SlotKeyA();
    break;

  case Qt::Key_S:
    std::cout << "keyPressEvent override    -   Key_S" << std::endl;
    emit SlotKeyS();
    break;

  case Qt::Key_D:
    std::cout << "keyPressEvent override    -   Key_D" << std::endl;
    emit SlotKeyD();
    break;

  case Qt::Key_Escape:
    qApp->quit();
    break;

  default:
    std::cout << "keyPressEvent override    -   default" << std::endl;
    break;
  }

  return;
}

void MainWindow::SlotKeyW()
{
  std::cout << "wUp   -   slot worked" << std::endl;
  emit SignalKeyW();
  ui->widget->update();
  window_score = GLWidget::score_changed();
  ui->scorenum->setText(QString::fromStdString(std::to_string(window_score)));

  return;
}

void MainWindow::SlotKeyA()
{
  std::cout << "aLeft    -    slot worked" << std::endl;
  emit SignalKeyA();
  ui->widget->update();
  window_score = GLWidget::score_changed();
  ui->scorenum->setText(QString::fromStdString(std::to_string(window_score)));

  return;
}

void MainWindow::SlotKeyS()
{
  std::cout << "sDown    -   slot worked" << std::endl;
  emit SignalKeyS();
  ui->widget->update();
  window_score = GLWidget::score_changed();
  ui->scorenum->setText(QString::fromStdString(std::to_string(window_score)));

  return;
}

void MainWindow::SlotKeyD()
{
  std::cout << "dRight    -   slot worked" << std::endl;
  emit SignalKeyD();
  ui->widget->update();
  window_score = GLWidget::score_changed();
  ui->scorenum->setText(QString::fromStdString(std::to_string(window_score)));

  return;
}

void MainWindow::UpdateScore()
{
  window_score = GLWidget::score_changed();
  ui->scorenum->setText(QString::fromStdString(std::to_string(window_score)));
}
