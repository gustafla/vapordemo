#ifdef GL_ES
    precision highp float;
#endif

varying vec2 v_texcoord;
varying vec3 v_light;
varying vec3 v_position;
uniform sampler2D u_textures[1];
uniform vec4 u_color;
uniform float u_opacity;

void main() {
    vec4 tcolor = texture2D(u_textures[0], v_texcoord);
    gl_FragColor = vec4(mix(tcolor.rgb, u_color.rgb, u_color.a) * v_light, tcolor.a*u_opacity);
}
