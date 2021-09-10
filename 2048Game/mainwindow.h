#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
  Q_OBJECT

protected:
  void keyPressEvent(QKeyEvent *event) override;

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  static int window_score;

private:
  Ui::MainWindow *ui;

public slots:
  //void Change_Score(int);
  void SlotKeyW();
  void SlotKeyA();
  void SlotKeyS();
  void SlotKeyD();
  void UpdateScore();

signals:
  void SignalKeyW();
  void SignalKeyA();
  void SignalKeyS();
  void SignalKeyD();
};

#endif // MAINWINDOW_H
