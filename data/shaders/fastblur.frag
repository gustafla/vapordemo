// Copyright 2015 Lauri Gustafsson
/*
This file is part of Low Quality is the Future.

    Low Quality is the Future is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Low Quality is the Future is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Low Quality is the Future, see COPYING. If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef GL_ES
precision highp float;
#endif

uniform vec2 u_resolution;
uniform sampler2D u_textures[1];

float offset[5];
float weight[5];

void main() {
    offset[0] = 0.0;
    offset[1] = 1.0;
    offset[2] = 2.0;
    offset[3] = 3.0;
    offset[4] = 4.0;
    weight[0] = 0.2270270270;
    weight[1] = 0.1945945946;
    weight[2] = 0.1216216216;
    weight[3] = 0.0540540541;
    weight[4] = 0.0162162162;

    vec3 fragmentColor;

    fragmentColor = texture2D( u_textures[0], gl_FragCoord.xy/u_resolution.xy ).rgb * weight[0];
    for (int i=0; i<5; i++) {
        fragmentColor += texture2D(u_textures[0], (gl_FragCoord.xy + vec2(0.0, offset[i])) / u_resolution.xy).rgb * weight[i];
        fragmentColor += texture2D(u_textures[0], (gl_FragCoord.xy - vec2(0.0, offset[i])) / u_resolution.xy).rgb * weight[i];
    }
    for (int i=0; i<5; i++) {
        fragmentColor += texture2D( u_textures[0], ( vec2(gl_FragCoord)+vec2(offset[i], 0.0) )/u_resolution.xy ).rgb * weight[i];
        fragmentColor += texture2D( u_textures[0], ( vec2(gl_FragCoord)-vec2(offset[i], 0.0) )/u_resolution.xy ).rgb * weight[i];
    }

    gl_FragColor = vec4(fragmentColor, 1.0);
}
