#ifdef GL_ES
precision highp float;
#endif

attribute vec3 a_position;
attribute vec2 a_texcoord;
attribute vec3 a_normal;

varying vec2 v_texcoord;
varying vec3 v_position;
varying vec3 v_normal;
varying vec3 v_light;

uniform mat4 u_m_projection;
uniform mat4 u_m_view;
uniform float u_time;

/*uniform vec3 u_lights_directional[N_LIGHTS_DIRECTIONAL*2+1];
uniform vec3 u_lights_point[N_LIGHTS_POINT*2+1];
uniform float u_light_ambient;*/

void main() {
    v_texcoord = a_texcoord;
    v_normal = a_normal;
    
    vec4 positionWorld = vec4(v_position, 1.0);
    //positionWorld.x = mod((positionWorld.x+10.0)+u_time, 20.0)-10.0;
    
    vec4 positionEye = u_m_view * positionWorld;
    float distFromCam = length(positionEye.xyz);
    v_light = vec3(1.0);//vec3(clamp(5.0/distFromCam, 0.0, 1.0));
    v_position = positionEye.xyz;
    
    gl_PointSize = 20.0/distFromCam;
    gl_Position = u_m_projection * positionEye;
}
