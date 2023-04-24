#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {}

GLWidget::~GLWidget() { clean(); }

void GLWidget::initializeGL() {
  //Инициализация, установка цветов, очистка буффера цветов
  setlocale(LC_NUMERIC, "C");
  initializeOpenGLFunctions();

  glClearColor(color_back.x(), color_back.y(), color_back.z(), color_back.w());
  glClear(GL_COLOR_BUFFER_BIT);
  //Добавление вершинного и пиксельного шейдера в программу, линковка и привязка
  program.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                  SRCDIR "v_shader.glsl");
  program.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                  SRCDIR "f_shader.glsl");
  program.link();
  program.bind();
  // Включение функций глубины и отбраковки задних граней
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  geometries = new GeometryDraw();
}

void GLWidget::resizeGL(int w, int h) {
  //Установка возможности менять масштаб окна сохраняя пропорции, включение
  //функции перспетивы
  qreal aspect = qreal(w) / qreal(h ? h : 1);
  const qreal zNear = 0.01, zFar = 100, fov = 45.0;
  glMatrixMode(GL_PROJECTION);
  projection.setToIdentity();
  if (projection_type == true) {
    QRectF orthoRectangle;
    orthoRectangle.setWidth(640);
    orthoRectangle.setHeight(480);
    projection.ortho(-5.0, 5.0, -5.0, 5.0, -100, 100);
  } else if (projection_type == false) {
    projection.perspective(fov, aspect, zNear, zFar);
  }
}

void GLWidget::paintGL() {
  //Очистка буффера цвета и глубины, установка цветов, очистка цветового буффера
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(color_back.x(), color_back.y(), color_back.z(), color_back.w());
  glClear(GL_COLOR_BUFFER_BIT);

  glLineWidth(line_width);
  setTypeLine();
  //Инициализация матрицы 4x4 приведение к единичной, позиционирование
  QMatrix4x4 matrix;
  matrix.setToIdentity();
  matrix.translate(0.0, 0.0, -10.0);
  matrix.rotate(rotation);
  //Установление матрицы в соотвествущее значение шейдерной программы
  program.setUniformValue("mvp_matrix", projection * matrix);
  //Отрисовка, таймер повтора отрисовки
  geometries->draw_polygon(&program);
  timer.start(13, this);
}

void GLWidget::set_color_back() {
  QColor color = QColorDialog::getColor();
  color_back =
      QVector4D(color.redF(), color.greenF(), color.blueF(), color.alphaF());
  update();
}

void GLWidget::set_ortho_projection(bool new_projection) {
  projection_type = new_projection;
  if (flag_open) {
    resizeGL(695, 785);
    update();
  }
}

void GLWidget::setTypeLine() {
  dotted                       ? glEnable(GL_LINE_STIPPLE),
      glLineStipple(3, 0xAAAA) : glDisable(GL_LINE_STIPPLE);
}

void GLWidget::mousePressEvent(QMouseEvent *e) {
  mousePressPosition = QVector2D(e->pos());
}

void GLWidget::mouseReleaseEvent(QMouseEvent *e) {
  QVector2D diff = QVector2D(e->pos()) - mousePressPosition;
  QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();
  qreal acc = diff.length() / 100.0;
  rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();
  angularSpeed += acc;
}

void GLWidget::mouseMoveEvent(QMouseEvent *e) {
  QVector2D diff = QVector2D(e->pos()) - mousePressPosition;
  QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();
  qreal acc = diff.length() / 100.0;
  rotationAxis = (rotationAxis * angularSpeed / 10 + n * acc).normalized();
  angularSpeed = acc;
}

void GLWidget::timerEvent(QTimerEvent *) {
  angularSpeed *= 0.99;
  if (angularSpeed < 0.01) {
    angularSpeed = 0.0;
  } else {
    rotation =
        QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;
    update();
  }
}

void GLWidget::clean() {
  makeCurrent();
  delete geometries;
  doneCurrent();
}
