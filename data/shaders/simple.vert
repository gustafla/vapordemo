#ifdef GL_ES
    precision highp float;
#endif

attribute vec3 a_position;
attribute vec2 a_texcoord;
attribute vec3 a_normal;
varying vec2 v_texcoord;
varying vec3 v_light;
varying vec3 v_position;
varying vec3 v_normal;

void main() {
    v_texcoord = a_texcoord;
    v_light = vec3(1.0);
    v_position = a_position;
    v_normal = normalize(vec3(0.0, 0.0, 1.0));
    gl_Position = vec4(a_position, 1.0);
}
