#include "oglgeometry.h"

GeometryDraw::GeometryDraw() : indexBuf(QOpenGLBuffer::IndexBuffer) {
  // Создание буфера массивов и индексов, инициализация объекта
  initializeOpenGLFunctions();
  arrayBuf.create();
  indexBuf.create();
  init_object();
}

GeometryDraw::~GeometryDraw() {
  arrayBuf.destroy();
  indexBuf.destroy();
  if (!file_path.isEmpty()) remove_object(&object);
}

void GeometryDraw::init_object() {
  if (this->file_path.isEmpty()) return;
  // Парсинг и привязка вершин
  object = parsing((char *)file_path.toStdString().c_str());
  binding_vertices();
  // Создание индексов типа vector<GLuint> помещение в них полигонов из объекта
  std::vector<GLuint> indices;
  for (uint i = 0; i < object.polygon.size; i++) {
    indices.push_back(object.polygon.polygon[i] - 1);
  }
  // Привязка и алокация индексного буфера, загрузка базовых настроек
  indexBuf.bind();
  indexBuf.allocate(indices.data(), indices.size() * sizeof(GLuint));
  load_settings();
}

void GeometryDraw::binding_vertices() {
  // Создание вершин типа vector<QVector3D> помещение в них координат из объекта
  std::vector<QVector3D> vertices;
  for (uint i = 0; i < object.matrix3d.rows; i++) {
    vertices.emplace_back(QVector3D(object.matrix3d.matrix[i][X],
                                    object.matrix3d.matrix[i][Y],
                                    object.matrix3d.matrix[i][Z]));
  }
  // Привязка и алокация буфера массивов
  arrayBuf.bind();
  arrayBuf.allocate(vertices.data(), vertices.size() * sizeof(QVector3D));
}

void GeometryDraw::load_settings() {
  QSettings _settings(SRCDIR "settings_demo.ini", QSettings::IniFormat);

  _settings.beginGroup("color_obj");
  double colorR = _settings.value("LineColorR", 0.0).toDouble();
  double colorG = _settings.value("LineColorG", 0.0).toDouble();
  double colorB = _settings.value("LineColorB", 0.0).toDouble();
  double colorA = _settings.value("LineColorA", 0.6).toDouble();

  double _colorR = _settings.value("VertexColorR", 0.0).toDouble();
  double _colorG = _settings.value("VertexColorG", 1.0).toDouble();
  double _colorB = _settings.value("VertexColorB", 0.0).toDouble();
  double _colorA = _settings.value("VertexColorA", 1.0).toDouble();
  _settings.endGroup();
  set_color_line(QVector4D(colorR, colorG, colorB, colorA));
  set_color_vertex(QVector4D(_colorR, _colorG, _colorB, _colorA));

  _settings.beginGroup("Vertex");
  uint vertex_size = _settings.value("VertexWidth", 0).toInt();
  uint vertex_type = _settings.value("VertexType", 0).toInt();
  _settings.endGroup();
  set_vertex_size(vertex_size);
  set_type_vertex(vertex_type);
}

void GeometryDraw::draw_polygon(QOpenGLShaderProgram *program) {
  if (this->file_path.isEmpty()) return;
  // Привязка вершин, буфера массивов и индексов установка соответствия с
  // вершинным и пиксельным шейдерами
  binding_vertices();
  arrayBuf.bind();
  indexBuf.bind();

  int vertexLocation = program->attributeLocation("a_position");
  program->enableAttributeArray(vertexLocation);
  program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3,
                              sizeof(QVector3D));

  int texcoordLocation = program->attributeLocation("a_texcoord");
  program->enableAttributeArray(texcoordLocation);
  program->setAttributeBuffer(texcoordLocation, GL_FLOAT, 0, 2,
                              sizeof(QVector2D));

  program->setUniformValue("color", color_line);
  // Отрисовка полигонов
  glDrawElements(GL_LINES, (indexBuf.size() / sizeof(GLuint)), GL_UNSIGNED_INT,
                 0);
  // Отрисовка вершин
  if (vertex_type) draw_vertices(program);
}

void GeometryDraw::draw_vertices(QOpenGLShaderProgram *program) {
  glPointSize(vertex_size);
  if (vertex_type == 2)
    glEnable(GL_POINT_SMOOTH);
  else
    glDisable(GL_POINT_SMOOTH);

  program->setUniformValue("color", color_vertex);
  glDrawElements(GL_POINTS, (indexBuf.size() / sizeof(GLuint)), GL_UNSIGNED_INT,
                 0);
}

void GeometryDraw::rotation_object(const double value, const int asix) {
  if (this->file_path.isEmpty()) return;

  rotation_matrix(&object.matrix3d, value - rotationPos[asix], asix);
  rotationPos[asix] = value;
  binding_vertices();
}

void GeometryDraw::move_object(const double value, const int asix) {
  if (this->file_path.isEmpty()) return;

  move_matrix(&object.matrix3d, value - movePos[asix], asix);
  movePos[asix] = value;
  binding_vertices();
}

void GeometryDraw::scaling(const double value) {
  if (this->file_path.isEmpty()) return;

  if (value > EPS) {
    zoom_matrix(&object.matrix3d, value / scale);
    scale = value;
    binding_vertices();
  }
}
