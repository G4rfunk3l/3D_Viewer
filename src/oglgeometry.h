#ifndef OGLGEOMETRY_H
#define OGLGEOMETRY_H

#include <qmath.h>

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QSettings>

extern "C" {
#include "3DViewer.h"
}

class GeometryDraw : public QOpenGLFunctions {
 public:
  GeometryDraw();
  virtual ~GeometryDraw();

  void init_object();
  void draw_polygon(QOpenGLShaderProgram *program);

  inline void set_filepath(const QString &new_file_path) {
    file_path = new_file_path;
  }
  inline void set_color_vertex(const QVector4D &new_color_v) {
    color_vertex = new_color_v;
  }
  inline void set_color_line(const QVector4D &new_color_l) {
    color_line = new_color_l;
  }
  inline void set_vertex_size(int new_vert_size) {
    vertex_size = new_vert_size;
  }
  inline void set_type_vertex(int new_type) { vertex_type = new_type; }

  inline size_t get_vertex_count() const { return object.count_of_vertexes; }
  inline size_t get_facets_count() const { return object.count_of_facets; }
  inline QVector4D get_color_vertex() const { return color_vertex; }
  inline QVector4D get_color_line() const { return color_line; }

  void rotation_object(double angle, int axis);
  void move_object(double distance, int axis);
  void scaling(double factor);

 private:
  void draw_vertices(QOpenGLShaderProgram *program);
  void binding_vertices();
  void load_settings();

  QOpenGLBuffer arrayBuf;
  QOpenGLBuffer indexBuf;
  QString file_path;
  object_t object;

  QVector4D color_line = QVector4D(0.3, 0.3, 0.3, 0.7);
  QVector4D color_vertex = QVector4D(0.0, 1.0, 0.0, 1.0);

  double rotationPos[AXIS] = {0.0, 0.0, 0.0};
  double movePos[AXIS] = {0.0, 0.0, 0.0};
  double scale = 1.0;

  int vertex_size = 0;
  int vertex_type = 0;
};

#endif  // OGLGEOMETRY_H
