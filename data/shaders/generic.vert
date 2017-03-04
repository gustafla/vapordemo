// Copyright 2016 Lauri Gustafsson
/*
This file is part of .

    ______DEMO______ is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ______DEMO______ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ______DEMO______, see COPYING. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef N_LIGHTS_DIRECTIONAL
#define N_LIGHTS_DIRECTIONAL 0
#endif

#ifndef N_LIGHTS_POINT
#define N_LIGHTS_POINT 0
#endif

#ifdef GL_ES
precision highp float;
#endif

attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_texcoord;

varying vec2 v_texcoord;
varying vec3 v_position;
varying vec3 v_normal;
varying vec3 v_light;

uniform mat4 u_m_projection;
uniform mat4 u_m_model;
uniform mat4 u_m_view;

uniform vec3 u_lights_directional[N_LIGHTS_DIRECTIONAL*2+1];
uniform vec3 u_lights_point[N_LIGHTS_POINT*2+1];
uniform float u_light_ambient;

float diffuseLight(vec3 dir, vec3 normal) {
    return max(dot(-normal, dir), 0.0);
}

vec3 lighting(vec3 pos, vec3 normal) {
    vec3 l = vec3(u_light_ambient);
    
    for (int i=0; i<N_LIGHTS_DIRECTIONAL*2; i+=2) {
        l += diffuseLight(u_lights_directional[i], normal) * u_lights_directional[i+1];
    }
    
    vec3 ray;
    for (int i=0; i<N_LIGHTS_POINT*2; i+=2) {
        ray = pos - u_lights_point[i];
        l += diffuseLight(normalize(ray), normal) * (1.0/length(ray)) * u_lights_point[i+1];
    }
    
    return clamp(l, 0.0, 1.0);
}

void main() {
    v_texcoord = a_texcoord;
    
    vec4 positionWorld = (u_m_model * vec4(a_position, 1.0));
    vec4 positionEye = (u_m_view * positionWorld);
    vec4 normalWorld = (u_m_model * vec4(a_normal, 0.0));
    vec4 normalEye = (u_m_view * normalWorld);
    v_position = positionEye.xyz;
    v_normal = normalEye.xyz;
    
    v_light = lighting(positionWorld.xyz, normalWorld.xyz);
    
    gl_Position = u_m_projection * positionEye;
}
