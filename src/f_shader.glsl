#ifdef GL_ES
precision highp int;
precision highp float;
#endif

uniform highp vec4 color;
varying vec2 v_texcoord;

void main()
{
    gl_FragColor = color;
}
