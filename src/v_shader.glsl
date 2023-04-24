#ifdef GL_ES
precision highp int;
precision highp float;
#endif

attribute vec4 a_position;
attribute vec2 a_texcoord;
uniform mat4 mvp_matrix;
varying vec2 v_texcoord;

void main()
{
    gl_Position = mvp_matrix * a_position;
    v_texcoord = a_texcoord;
}
