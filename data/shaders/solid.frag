#ifdef GL_ES
    precision highp float;
#endif

varying vec2 v_texcoord;
varying vec3 v_light;
varying vec3 v_position;
varying vec3 v_normal;
uniform vec4 u_color;
uniform float u_opacity;

void main() {
    gl_FragColor = vec4(u_color.rgb * v_light, u_opacity);
}
