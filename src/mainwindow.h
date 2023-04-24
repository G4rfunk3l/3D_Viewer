#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QColor>
#include <QColorDialog>
#include <QDebug>
#include <QFileDialog>
#include <QMainWindow>
#include <QSettings>
#include <QSlider>
#include <QSpinBox>
#include <QTimer>
#include <QToolTip>

#include "QtCore/qdatetime.h"
#include "glwidget.h"
#include "qgifimage.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 protected:
  QString SelectDir();
  void showSavedToolTip();
  void showGifToolTip(int second);
  void set_start_value_slider();

 private slots:

  void OpenFile();
  void CloseViewer();

  void SaveSettings();
  void LoadSettings();

  void VertexColor();
  void EdgeColor();

  void MakeGif();
  void RecordGif();
  void MakeScreenshot();

  void on_horizontalSlider_X_move_valueChanged(int);
  void on_horizontalSlider_Y_move_valueChanged(int);
  void on_horizontalSlider_Z_move_valueChanged(int);
  void on_horizontalSlider_scale_valueChanged(int);

  void on_dial_X_rotation_valueChanged(int value);
  void on_dial_Y_rotation_valueChanged(int value);
  void on_dial_Z_rotation_valueChanged(int value);

  void on_comboBox_VertexType_currentIndexChanged(int);
  void on_spinBox_VertexWidth_valueChanged(int);
  void on_doubleSpinBox_scale_valueChanged(double arg1);

 private:
  int timerCounter = 0;
  QGifImage *gif;
  QTimer *timer;
  QString *path;
  bool flag_open = false;

  Ui::MainWindow *ui;
};

#endif  // MAINWINDOW_H
