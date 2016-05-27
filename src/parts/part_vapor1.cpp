#include "part_vapor1.hpp"
#include "3dapp_tga_file.hpp"
#include "3dapp_obj_file.hpp"
#include "3dapp_paths.hpp"
#include "3dapp_gfx_utils.hpp"
#include "3dapp_uniforms.hpp"
#include "3dapp_matrices.hpp"
#include "3dapp_geo_primitives.hpp"
#include "3dapp_util.hpp"
#include <cmath>

#define PI2 (2*DEMO_PI)

static const Light pointLights[] = {
};

static const Light directionalLights[] = {
    Light(vec3(0.2, -0.2, -1.0), vec3(0.6, 0.6, 0.6))
};

PartVapor1::PartVapor1(float t):
DemoPart(t),
lights(0.2f, pointLights, sizeof(pointLights)/sizeof(pointLights[0]), directionalLights, sizeof(directionalLights)/sizeof(directionalLights[0])),
pillarTexture(loadTGAFile(texturePath("marble.tga"))),
shaderMvp(Shader::loadFromFile(shaderPath("generic.vert")), Shader::loadFromFile(shaderPath("generic.frag"))), 
frac(loadTGAFile(texturePath("trianglefrac.tga"))),
cloud(loadTGAFile(texturePath("cloud.tga"))),
mvp(getPProjMat(45, DEMO_W/DEMO_H, 0.1, 10.0)) {
    genMeshes();
    
    shader = new Program(Shader::loadFromFile(shaderPath("generic.vert"), lights), Shader::loadFromFile(shaderPath("generic.frag"), lights));
    shader->use();
    
    setBaseUniforms(*shader, 1, lights);
    setBaseUniforms(shaderMvp);
    
    for (int i=0; i<NUM_CLOUDS; i++) {
        clouds[i*STR_CLOUDS]   = randf()*3.0;
        clouds[i*STR_CLOUDS+1] = randf()*3.0;
        clouds[i*STR_CLOUDS+2] = randf();
        clouds[i*STR_CLOUDS+3] = randf()*0.5+0.5;
        clouds[i*STR_CLOUDS+4] = randf()*DEMO_PI;
    }
}

PartVapor1::~PartVapor1() {
    delete pillar;
    delete shader;
}

void PartVapor1::draw() {
    Sync& sync = Demo::singleton().getSync();
    
    vec4 cloudColor1 = vec4(0.6, 0.6, 0.1, 1);
    vec4 cloudColor2 = vec4(0.0, 0.6, 0.1, 1);
    mvp.reset();
    cloud.bindToUnit(0);
    for(int i=0; i<NUM_CLOUDS; i++) {
        mvp.setModel(clouds[i*STR_CLOUDS]+sin(DEMO_T()*sin(clouds[i*STR_CLOUDS]))*0.3, clouds[i*STR_CLOUDS+1]+sin(DEMO_T()+sin(clouds[i*STR_CLOUDS]))*sin(DEMO_T()*0.2+sin(clouds[i*STR_CLOUDS+1])*2.0)*0.1, clouds[i*STR_CLOUDS+2]-4.0+sin(clouds[i*STR_CLOUDS]*2.0+DEMO_T()*0.7), 0.0, 0.0, clouds[i*STR_CLOUDS+4], 1.0);
        mvp.apply(shaderMvp);
        setColorUniform(shaderMvp, mix(cloudColor1, cloudColor2, clouds[i*STR_CLOUDS+3]));
        GeoPrimitives::singleton().quad.draw(shaderMvp);
    }
    glClear(GL_DEPTH_BUFFER_BIT);
    
    mvp.reset();
    mvp.setView(0.0, 0.0, -4.0, 0.0, 0.0, 0.0);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    
    pillarTexture.bindToUnit(0);
    shader->use();
    
    mvp.setModelRotation(0.0, DEMO_T()*0.6, 0.1);
    mvp.setModelTranslation(sync.getValue(SYNC_PART_VAPOR1_PILLAR_XSEP, DEMO_T()), 0.0, 0.0);
    mvp.apply(*shader);
    pillar->draw(*shader);
    
    mvp.setModelRotation(0.0, DEMO_T(), 0.2);
    mvp.setModelTranslation(-sync.getValue(SYNC_PART_VAPOR1_PILLAR_XSEP, DEMO_T()), 0.0, 0.0);
    mvp.apply(*shader);
    pillar->draw(*shader);
    
    glDisable(GL_DEPTH_TEST);
    
    glClear(GL_DEPTH_BUFFER_BIT);
    
    vec4 ca = vec4(DEMO_C_DBLUE, 1.0);
	vec4 cb = vec4(DEMO_C_VPINK, 1.0);
    shaderMvp.use();
    frac.bindToUnit(0);
    
    mvp.reset();
    float I=18;
    for(int i=0; i<I; i++) {
		mvp.setModel(sync.getValue(SYNC_PART_TRIANGLES_TUNNEL_DEPTHPAN, DEMO_T())*pow((1.0-i/I), 2.0), 0.0, fmod((i*1.0f)+sync.getValue(SYNC_PART_TRIANGLES_TUNNEL_DEPTH, DEMO_T()), I)-I+1, 0.0, sync.getValue(SYNC_PART_TRIANGLES_MAIN_ROTATION, DEMO_T()), (2*3.1415)*0.125+sync.getValue(SYNC_PART_TRIANGLES_TUNNEL_DEPTHROT, DEMO_T())*(i/I));
		mvp.setViewTranslation(sync.getValue(SYNC_PART_TRIANGLES_VIEW_X, DEMO_T()), sync.getValue(SYNC_PART_TRIANGLES_VIEW_Y, DEMO_T()));
        mvp.apply(shaderMvp);
        setColorUniform(shaderMvp, mix(ca, cb, sin(i/(I)*3.1415*2.0)*0.5+0.5));
		GeoPrimitives::singleton().quad.draw(shaderMvp);
	}
}

float getW(float y, float r) {
    return 0.3*std::cos(y*0.2*DEMO_PI)+std::sin(r*16.0)*0.01;
}

float getWEnd(float y, float r) {
    return 0.45;
}

void pushTrianglePositionNormal(Mesh& mesh, vec3 a, vec3 b, vec3 c) {
    mesh.pushPosition(a);
    mesh.pushPosition(b);
    mesh.pushPosition(c);
    vec3 normal = triangleNormal(a, b, c);
    for (int i=0; i<3; i++)
        mesh.pushNormal(normal);
}

void fillCylindricalMesh(Mesh& mesh, const float STRIP_H, const float STRIP_W, float start, float stop, float radiusFunc(float,float)) {
    for (float y=start; y<stop; y+=STRIP_H) {
        for (float r=0.0; r<PI2; r+=STRIP_W) {
            pushTrianglePositionNormal( mesh,
                vec3(std::sin(r)*radiusFunc(y,r), y, std::cos(r)*radiusFunc(y,r)),
                vec3(std::sin(r+STRIP_W)*radiusFunc(y,r+STRIP_W), y, std::cos(r+STRIP_W)*radiusFunc(y,r+STRIP_W)),
                vec3(std::sin(r+STRIP_W)*radiusFunc(y+STRIP_H,r+STRIP_W), y+STRIP_H, std::cos(r+STRIP_W)*radiusFunc(y+STRIP_H,r+STRIP_W))
            );
            mesh.pushTexcoord(vec2(((r)/PI2), (y+1)/2));
            mesh.pushTexcoord(vec2(((r+STRIP_W)/PI2), (y+1)/2));
            mesh.pushTexcoord(vec2(((r+STRIP_W)/PI2), (y+STRIP_H+1)/2));
                
            pushTrianglePositionNormal( mesh,
                vec3(std::sin(r+STRIP_W)*radiusFunc(y+STRIP_H,r+STRIP_W), y+STRIP_H, std::cos(r+STRIP_W)*radiusFunc(y+STRIP_H,r+STRIP_W)),
                vec3(std::sin(r)*radiusFunc(y+STRIP_H,r), y+STRIP_H, std::cos(r)*radiusFunc(y+STRIP_H,r)),
                vec3(std::sin(r)*radiusFunc(y,r), y, std::cos(r)*radiusFunc(y,r))
            );
            mesh.pushTexcoord(vec2(((r+STRIP_W)/PI2), (y+STRIP_H+1)/2));
            mesh.pushTexcoord(vec2(((r)/PI2), (y+STRIP_H+1)/2));
            mesh.pushTexcoord(vec2(((r)/PI2), (y+1)/2));
        }
    }
}

void fillCircularMesh(Mesh& mesh, bool up, const float STRIP_W, float y, float radius) {
    vec3 a, b, c;
    for (float r=0.0; r<PI2; r+=STRIP_W) {
        if (up) {
            a = vec3(0.0f, y, 0.0f);
            b = vec3(std::cos(r+STRIP_W)*radius, y, std::sin(r+STRIP_W)*radius);
            c = vec3(std::cos(r)*radius, y, std::sin(r)*radius);
            mesh.pushTexcoord(vec2(0.5, 0.5));
            mesh.pushTexcoord(vec2(std::cos(r+STRIP_W)/2+1, std::sin(r+STRIP_W)/2+1));
            mesh.pushTexcoord(vec2(std::cos(r)/2+1, std::sin(r)/2+1));
        } else {
            a = vec3(0.0f, y, 0.0f);
            c = vec3(std::sin(r)*radius, y, std::cos(r)*radius);
            b = vec3(std::sin(r+STRIP_W)*radius, y, std::cos(r+STRIP_W)*radius);
            mesh.pushTexcoord(vec2(0.5, 0.5));
            mesh.pushTexcoord(vec2(std::cos(r)/2+1, std::sin(r)/2+1));
            mesh.pushTexcoord(vec2(std::cos(r+STRIP_W)/2+1, std::sin(r+STRIP_W)/2+1));
        }
        pushTrianglePositionNormal(mesh, a, b, c);
    }
}

void fillPlanarMesh(Mesh& mesh, bool up, const float STRIP_W, const float STRIP_H, float height, float w, float h) {
    vec3 a, b, c;
    if (up) {
        for (float x=-w/2; x<w/2; x+=STRIP_W) {
            for (float y=-h/2; y<h/2; y+=STRIP_H) {
                a = vec3(x, height, y);
                b = vec3(x+STRIP_W, height, y+STRIP_H);
                c = vec3(x+STRIP_W, height, y);
                mesh.pushTexcoord(vec2(x/w+0.5, y/w+0.5));
                mesh.pushTexcoord(vec2((x+STRIP_W)/w+0.5, (y+STRIP_H)/w+0.5));
                mesh.pushTexcoord(vec2((x+STRIP_W)/w+0.5, y/w+0.5));
                pushTrianglePositionNormal(mesh, a, b, c);
                
                a = vec3(x, height, y);
                b = vec3(x, height, y+STRIP_H);
                c = vec3(x+STRIP_W, height, y+STRIP_H);
                mesh.pushTexcoord(vec2(x/w+0.5, y/w+0.5));
                mesh.pushTexcoord(vec2(x/w+0.5, (y+STRIP_H)/w+0.5));
                mesh.pushTexcoord(vec2((x+STRIP_W)/w+0.5, (y+STRIP_H)/w+0.5));
                pushTrianglePositionNormal(mesh, a, b, c);
            }
        }
    } else {
        for (float x=-w/2; x<w/2; x+=STRIP_W) {
            for (float y=-h/2; y<h/2; y+=STRIP_H) {
                a = vec3(x, height, y);
                b = vec3(x+STRIP_W, height, y);
                c = vec3(x+STRIP_W, height, y+STRIP_H);
                mesh.pushTexcoord(vec2(x/w+0.5, y/w+0.5));
                mesh.pushTexcoord(vec2((x+STRIP_W)/w+0.5, y/w+0.5));
                mesh.pushTexcoord(vec2((x+STRIP_W)/w+0.5, (y+STRIP_H)/w+0.5));
                pushTrianglePositionNormal(mesh, a, b, c);
                
                a = vec3(x, height, y);
                b = vec3(x+STRIP_W, height, y+STRIP_H);
                c = vec3(x, height, y+STRIP_H);
                mesh.pushTexcoord(vec2(x/w+0.5, y/w+0.5));
                mesh.pushTexcoord(vec2((x+STRIP_W)/w+0.5, (y+STRIP_H)/w+0.5));
                mesh.pushTexcoord(vec2(x/w+0.5, (y+STRIP_H)/w+0.5));
                pushTrianglePositionNormal(mesh, a, b, c);
            }
        }
    }
}

void PartVapor1::genPillarMesh() {
    Mesh mesh;
    
    const float STRIP_H = 0.5;
    const float STRIP_W = 0.1;
    const float HALF_PI = DEMO_PI/2.0;
    
    fillCylindricalMesh(mesh, STRIP_H, STRIP_W, -1.0, 1.0, getW);
    fillCylindricalMesh(mesh, 0.1, HALF_PI, -1.1, -0.9, getWEnd);
    fillCylindricalMesh(mesh, 0.1, HALF_PI, 0.9, 1.1, getWEnd);
    fillCircularMesh(mesh, true, HALF_PI, 1.1, 0.45);
    fillCircularMesh(mesh, false, HALF_PI, 0.9, 0.45);
    fillCircularMesh(mesh, true, HALF_PI, -0.9,0.45);
    fillCircularMesh(mesh, false, HALF_PI, -1.1, 0.45);
    
    pillar = new StaticModel(mesh);
}

void PartVapor1::genMeshes() {
    genPillarMesh();
}
