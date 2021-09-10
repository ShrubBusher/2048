#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QWidget>
#include <QtOpenGL>
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QKeyEvent>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
  Q_OBJECT

public:

  explicit GLWidget(QWidget *parent = nullptr);

  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int w, int h) override;

  void DrawField();

public slots:
  void RestartSlot();
  void MoveUp();
  void MoveLeft();
  void MoveDown();
  void MoveRight();
  static int score_changed();
  static bool game_end_changed();
};



#endif // GLWIDGET_H
