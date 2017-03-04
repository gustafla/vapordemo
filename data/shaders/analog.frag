#ifdef GL_ES
    precision highp float;
#endif

varying vec2 v_texcoord;
uniform sampler2D u_textures[2];
uniform float u_time;

//uniform vec3 hue;
uniform float contrast;
uniform float brightness;
uniform float blur;
uniform float glitchiness;

#define t u_time*10.0

//#define contrast 1.1
//#define brightness 0.0
#define vignetteMult 0.3
//#define blur 0.6

float rand(vec2 co){
    return fract(sin(dot(co.xy, vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
    vec2 uv = v_texcoord;
    float vignette = length(vec2(0.5)-v_texcoord)*vignetteMult;
    
    float bottomMult = (1.0/uv.y*0.0005);
    
    //Line shifting to mimic VHS bottom artifacts
    uv.x -= abs(bottomMult*sin(uv.y*10.0+t)*sin(uv.y*22.0+t*1.1)*0.5+bottomMult*rand(uv.yy+u_time*0.1));
    
    //Wavy, like VAPOR wave-y XDDDDDDD funniest joke ever xaxaxaxaxa
    //float wa = max((sin(uv.y*4.0+sin(t*0.1)*3.0)-0.6), 0.0)*(sin(uv.y*30.0+t)+sin(uv.y*17.4+t*0.8))*(glitchiness-0.1)*0.01;
    //uv.x += wa;
    
    //Random glitchy scan lines
    float glitchLines = max(rand(uv.yy+u_time*0.06)-(1.0-glitchiness*0.01), 0.0);
    uv.x += glitchLines*3000.0;
    
    vec3 tcolor = texture2D(u_textures[0], uv).rgb;
    
    //Mix blur
    tcolor = mix(tcolor, texture2D(u_textures[1], uv).rgb, 0.0);
    
    //Picture adjustments
    tcolor = tcolor*contrast+brightness;
    
    //tcolor += vec3(wa*rand(uv.yy+t)*100.0);
    
    //Glitch line color mod
    tcolor.rb *= 1.0-(glitchLines*20000.0);
    
    //Harsh luminance noise in the bottom
    float noise = rand(uv+u_time*0.1);
    float noise2 = rand(uv+u_time*0.11);
    float noise3 = rand(uv+u_time*0.12)*2.0-1.0;
    tcolor += min((max(noise-0.9, 0.0)*bottomMult), 1.0)*100.0;
    
    //General noise
    tcolor.r += noise*(0.04+0.01*glitchiness);
    tcolor.g += noise2*(0.04+0.01*glitchiness);
    tcolor.b += noise3*(0.04+0.01*glitchiness);
    
    //Noisy specs
    tcolor += max(noise3-(1.0-glitchiness*0.01), 0.0)*200.0;
    
    //gl_FragColor = vec4(pow(tcolor, vec3(1.0/2.1))-vignette, 1.0);
    gl_FragColor = vec4(texture2D(u_textures[1], uv).rgb,1.0);
}
