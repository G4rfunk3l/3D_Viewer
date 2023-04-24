#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)

{
  //Инициализация основного окна, устновка групп и соединений для кнопок меню,
  //загрузка настроек
  ui->setupUi(this);
  ui->GroupFormatScreen->setExclusive(true);
  ui->GroupTypeLine->setExclusive(true);
  ui->GroupTypeProjection->setExclusive(true);

  connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(OpenFile()));
  connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(CloseViewer()));

  connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(SaveSettings()));
  connect(ui->actionLoad, SIGNAL(triggered()), this, SLOT(LoadSettings()));

  connect(ui->actionVertex, SIGNAL(triggered()), this, SLOT(VertexColor()));
  connect(ui->actionEdge, SIGNAL(triggered()), this, SLOT(EdgeColor()));
  connect(ui->actionBackground, &QAction::triggered, ui->openGLWidget,
          &GLWidget::set_color_back);

  connect(ui->actionSave_screenshot, SIGNAL(triggered()), this,
          SLOT(MakeScreenshot()));
  connect(ui->actionRecord_GIF, SIGNAL(triggered()), this, SLOT(MakeGif()));

  connect(ui->actionCentral, &QAction::triggered, ui->openGLWidget,
          &GLWidget::set_perspective_projection);
  connect(ui->actionParallel, &QAction::triggered, ui->openGLWidget,
          &GLWidget::set_ortho_projection);

  connect(ui->actionDotted, &QAction::triggered, ui->openGLWidget,
          &GLWidget::set_dotted_line);
  connect(ui->actionSolid, &QAction::triggered, ui->openGLWidget,
          &GLWidget::set_solid_line);

  connect(ui->spinBox_EdgesWidth, QOverload<int>::of(&QSpinBox::valueChanged),
          ui->openGLWidget, &GLWidget::set_line_width);
  connect(ui->spinBox_move_x, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &MainWindow::on_horizontalSlider_X_move_valueChanged);
  connect(ui->spinBox_move_y, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &MainWindow::on_horizontalSlider_Y_move_valueChanged);
  connect(ui->spinBox_move_z, QOverload<int>::of(&QSpinBox::valueChanged), this,
          &MainWindow::on_horizontalSlider_Z_move_valueChanged);
  connect(ui->spinBox_rotation_x, QOverload<int>::of(&QSpinBox::valueChanged),
          this, &MainWindow::on_dial_X_rotation_valueChanged);
  connect(ui->spinBox_rotation_y, QOverload<int>::of(&QSpinBox::valueChanged),
          this, &MainWindow::on_dial_Y_rotation_valueChanged);
  connect(ui->spinBox_rotation_z, QOverload<int>::of(&QSpinBox::valueChanged),
          this, &MainWindow::on_dial_Z_rotation_valueChanged);

  LoadSettings();
}

void MainWindow::set_start_value_slider() {
  ui->horizontalSlider_X_move->setValue(0);
  ui->horizontalSlider_Y_move->setValue(0);
  ui->horizontalSlider_Z_move->setValue(0);
  ui->dial_X_rotation->setValue(0);
  ui->dial_Y_rotation->setValue(0);
  ui->dial_Z_rotation->setValue(0);
  ui->horizontalSlider_scale->setValue(500);
}

MainWindow::~MainWindow() {
  //Сохранение настроек при выходе
  SaveSettings();
  delete ui;
}

void MainWindow::OpenFile() {
  //Открытие файла,установка стартовых значений, инициализация obj файла,
  //установка количества вершин, граней, имени файла, отрисовка
  //  SaveSettings();
  QString str = QFileDialog::getOpenFileName(this, "Choose obj file",
                                             QDir::homePath(), "*.obj");

  if (!str.isEmpty()) {
    flag_open = true;
    ui->openGLWidget->flag_open = true;
    set_start_value_slider();
    ui->openGLWidget->geometries->set_filepath(str);
    ui->openGLWidget->geometries->init_object();
    ui->label_VertexCount_value->setText(
        QString::number(ui->openGLWidget->geometries->get_vertex_count()));
    ui->label_EdgesCount_value->setText(
        QString::number(ui->openGLWidget->geometries->get_facets_count()));
    ui->label_fieldFilename->setText(QFileInfo(str).fileName());
    ui->openGLWidget->geometries->scaling(0.500);
    ui->openGLWidget->update();
  }
}

void MainWindow::CloseViewer() { close(); }

void MainWindow::SaveSettings() {
  QSettings settings(SRCDIR "settings_demo.ini", QSettings::IniFormat);

  settings.beginGroup("projection");
  settings.setValue("IsCentral_projection", ui->actionCentral->isChecked());
  settings.endGroup();

  settings.beginGroup("Edges");
  settings.setValue("EdgesWidth", ui->spinBox_EdgesWidth->value());
  settings.endGroup();

  settings.beginGroup("Vertex");
  settings.setValue("VertexWidth", ui->spinBox_VertexWidth->value());
  settings.setValue("VertexType", ui->comboBox_VertexType->currentIndex());
  settings.endGroup();

  settings.beginGroup("LineType");
  settings.setValue("IsLineType1", ui->actionSolid->isChecked());
  settings.endGroup();

  settings.beginGroup("PicType");
  settings.setValue("IsPicType1", ui->actionjpeg->isChecked());
  settings.endGroup();

  settings.beginGroup("color");
  settings.setValue("BackgroundColorR", ui->openGLWidget->get_color_back().x());
  settings.setValue("BackgroundColorG", ui->openGLWidget->get_color_back().y());
  settings.setValue("BackgroundColorB", ui->openGLWidget->get_color_back().z());
  settings.setValue("BackgroundColorA", ui->openGLWidget->get_color_back().w());
  settings.endGroup();

  settings.beginGroup("color_obj");
  settings.setValue("LineColorR",
                    ui->openGLWidget->geometries->get_color_line().x());
  settings.setValue("LineColorG",
                    ui->openGLWidget->geometries->get_color_line().y());
  settings.setValue("LineColorB",
                    ui->openGLWidget->geometries->get_color_line().z());
  settings.setValue("LineColorA",
                    ui->openGLWidget->geometries->get_color_line().w());

  settings.setValue("VertexColorR",
                    ui->openGLWidget->geometries->get_color_vertex().x());
  settings.setValue("VertexColorG",
                    ui->openGLWidget->geometries->get_color_vertex().y());
  settings.setValue("VertexColorB",
                    ui->openGLWidget->geometries->get_color_vertex().z());
  settings.setValue("VertexColorA",
                    ui->openGLWidget->geometries->get_color_vertex().w());
  settings.endGroup();
}

void MainWindow::LoadSettings() {
  QSettings settings(SRCDIR "settings_demo.ini", QSettings::IniFormat);

  settings.beginGroup("projection");
  bool projection = settings.value("IsCentral_projection", 1).toBool();
  settings.endGroup();
  ui->actionCentral->setChecked(projection);
  ui->actionParallel->setChecked(!projection);
  ui->openGLWidget->set_ortho_projection(!projection);

  settings.beginGroup("Edges");
  auto EdgesWidth = settings.value("EdgesWidth", 1).toInt();
  settings.endGroup();
  ui->spinBox_EdgesWidth->setValue(EdgesWidth);

  settings.beginGroup("Vertex");
  auto VertexWidth = settings.value("VertexWidth", 2).toInt();
  auto VertexType = settings.value("VertexType", 1).toInt();
  settings.endGroup();
  ui->spinBox_VertexWidth->setValue(VertexWidth);
  ui->comboBox_VertexType->setCurrentIndex(VertexType);

  settings.beginGroup("LineType");
  bool LineType = settings.value("IsLineType1", 1).toBool();
  settings.endGroup();
  ui->actionSolid->setChecked(LineType);
  ui->actionDotted->setChecked(!LineType);
  ui->openGLWidget->set_dotted_line(!LineType);

  settings.beginGroup("PicType");
  bool PicType = settings.value("IsPicType1", 1).toBool();
  settings.endGroup();
  ui->actionjpeg->setChecked(PicType);
  ui->actionbmp->setChecked(!PicType);

  settings.beginGroup("color");
  auto colorR = settings.value("BackgroundColorR", 1.0).toDouble();
  auto colorG = settings.value("BackgroundColorG", 1.0).toDouble();
  auto colorB = settings.value("BackgroundColorB", 1.0).toDouble();
  auto colorA = settings.value("BackgroundColorA", 1.0).toDouble();
  ui->openGLWidget->load_color_back(QVector4D(colorR, colorG, colorB, colorA));
  settings.endGroup();
}

void MainWindow::VertexColor() {
  QColor color = QColorDialog::getColor();
  ui->openGLWidget->geometries->set_color_vertex(
      QVector4D(color.redF(), color.greenF(), color.blueF(), color.alphaF()));
  ui->openGLWidget->update();
}

void MainWindow::EdgeColor() {
  QColor color = QColorDialog::getColor();
  ui->openGLWidget->geometries->set_color_line(
      QVector4D(color.redF(), color.greenF(), color.blueF(), color.alphaF()));
  ui->openGLWidget->update();
}

void MainWindow::MakeScreenshot() {
  QString path = SelectDir();
  QString EXT = ui->actionjpeg->isChecked() ? "jpeg" : "bmp";
  ui->openGLWidget->grab().save(path + "/" + QDate::currentDate().toString() +
                                " " + QTime::currentTime().toString() + "." +
                                EXT);
  showSavedToolTip();
}

QString MainWindow::SelectDir() {
  return QFileDialog::getExistingDirectory(
      this, tr("Open Directory"), SRCDIR "../media",
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
}

void MainWindow::MakeGif() {
  path = new QString(SelectDir());
  gif = new QGifImage();
  timerCounter = 0;
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(RecordGif()));
  timer->start(100);
}

void MainWindow::RecordGif() {
  if (timerCounter % 10 == 0) {
    showGifToolTip(5 - (timerCounter / 10));
  }
  ++timerCounter;

  gif->addFrame(ui->openGLWidget->grabFramebuffer(), 100);

  if (timerCounter == 50) {
    timer->stop();
    gif->save(*path + "/" + QDate::currentDate().toString() + " " +
              QTime::currentTime().toString() + ".gif");
    showSavedToolTip();
    delete gif;
  }
}

void MainWindow::showSavedToolTip() {
  QToolTip::showText(ui->pushButton_SaveText->mapToGlobal(QPoint()),
                     "File saved success!");  //вспомогательные сообщения
}

void MainWindow::showGifToolTip(int second) {
  QToolTip::showText(ui->pushButton_SaveText->mapToGlobal(QPoint()),
                     "Attention! Screen recording is in progress..." +
                         QString::number(second) + " seconds left");
}

void MainWindow::on_horizontalSlider_X_move_valueChanged(int value) {
  double value_d = (double)value / 7;
  ui->spinBox_move_x->setValue(value);
  ui->horizontalSlider_X_move->setValue(value);
  ui->openGLWidget->geometries->move_object(value_d, X);
  ui->openGLWidget->update();
}

void MainWindow::on_horizontalSlider_Y_move_valueChanged(int value) {
  double value_d = (double)value / 7;
  ui->spinBox_move_y->setValue(value);
  ui->horizontalSlider_Y_move->setValue(value);
  ui->openGLWidget->geometries->move_object(value_d, Y);
  ui->openGLWidget->update();
}

void MainWindow::on_horizontalSlider_Z_move_valueChanged(int value) {
  double value_d = (double)value / 7;
  ui->spinBox_move_z->setValue(value);
  ui->horizontalSlider_Z_move->setValue(value);
  ui->openGLWidget->geometries->move_object(value_d, Z);
  ui->openGLWidget->update();
}

void MainWindow::on_horizontalSlider_scale_valueChanged(int value) {
  double value_d = (double)value / 1000;
  ui->doubleSpinBox_scale->setValue(value_d);
  ui->openGLWidget->geometries->scaling(value_d);
  ui->openGLWidget->update();
}

void MainWindow::on_comboBox_VertexType_currentIndexChanged(int index) {
  if (flag_open) {
    ui->openGLWidget->geometries->set_type_vertex(index);
    ui->openGLWidget->update();
  }
}

void MainWindow::on_spinBox_VertexWidth_valueChanged(int arg1) {
  if (flag_open) {
    ui->openGLWidget->geometries->set_vertex_size(arg1);
    ui->openGLWidget->update();
  }
}

void MainWindow::on_doubleSpinBox_scale_valueChanged(double arg1) {
  ui->horizontalSlider_scale->setValue(arg1 * 1000);
}

void MainWindow::on_dial_X_rotation_valueChanged(int value) {
  double value_d = (double)value / 57.2958;
  ui->spinBox_rotation_x->setValue(value);
  ui->dial_X_rotation->setValue(value);
  ui->openGLWidget->geometries->rotation_object(value_d, X);
  ui->openGLWidget->update();
}

void MainWindow::on_dial_Y_rotation_valueChanged(int value) {
  double value_d = (double)value / 57.2958;
  ui->spinBox_rotation_y->setValue(value);
  ui->dial_Y_rotation->setValue(value);
  ui->openGLWidget->geometries->rotation_object(value_d, Y);
  ui->openGLWidget->update();
}

void MainWindow::on_dial_Z_rotation_valueChanged(int value) {
  double value_d = (double)value / 57.2958;
  ui->spinBox_rotation_z->setValue(value);
  ui->dial_Z_rotation->setValue(value);
  ui->openGLWidget->geometries->rotation_object(value_d, Z);
  ui->openGLWidget->update();
}
