#include "part_vapor1.hpp"
#include "tga_file.hpp"
#include "obj_file.hpp"
#include "paths.hpp"
#include "gfx_utils.hpp"
#include <cmath>

#define PI2 (2*DEMO_PI)

PartVapor1::PartVapor1(float t):
DemoPart(t),
pillarTexture(loadTGAFile(texturePath("marble.tga"))),
shader(Shader(shaderPath("generic.vert")), Shader(shaderPath("generic.frag"))),
mvp(getPProjMat(45, DEMO_W/DEMO_H, 0.1, 10.0)) {
    genPillarMesh();
    shader.use();
    
    setTextureUniforms(shader);
    setLightingUniforms(shader, 0.2f);
    
    glUniform4f(shader.getUfmHandle("color"), 0.0 , 0.0, 0.0, 0.0);
    mvp.setView(0.0, 0.0, -4.0, 0.0, 0.0, 0.0);
}

PartVapor1::~PartVapor1() {
    delete pillar;
}

void PartVapor1::draw() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    
    shader.use();
    pillarTexture.bindToUnit(0);
    mvp.setModelRotation(0.0, DEMO_T()*0.6, 0.1);
    mvp.setModelTranslation(0.8, 0.0, 0.0);
    mvp.buildModel();
    mvp.buildMVP();
    glUniformMatrix4fv(shader.getUfmHandle("mvp"), 1, GL_FALSE, mvp.getMVPArray());
    glUniformMatrix4fv(shader.getUfmHandle("model_rotation"), 1, GL_FALSE, mvp.getModelRotationArray());
    pillar->draw(shader);
    
    mvp.setModelRotation(0.0, DEMO_T(), 0.8);
    mvp.setModelTranslation(-0.8, 0.0, 0.0);
    mvp.buildModel();
    mvp.buildMVP();
    glUniformMatrix4fv(shader.getUfmHandle("mvp"), 1, GL_FALSE, mvp.getMVPArray());
    glUniformMatrix4fv(shader.getUfmHandle("model_rotation"), 1, GL_FALSE, mvp.getModelRotationArray());
    pillar->draw(shader);
    
    glDisable(GL_DEPTH_TEST);
}

float getW(float y, float r) {
    return 0.3*std::cos(y*0.2*DEMO_PI)+std::sin(r*16.0)*0.01;
}

float getWEnd(float y, float r) {
    return 0.45;
}

void pushTrianglePositionNormal(Mesh& mesh, vec4 a, vec4 b, vec4 c) {
    mesh.pushPosition(a);
    mesh.pushPosition(b);
    mesh.pushPosition(c);
    vec4 normal = triangleNormal(a, b, c);
    for (int i=0; i<3; i++)
        mesh.pushNormal(normal);
}

void fillCylindricalMesh(Mesh& mesh, const float STRIP_H, const float STRIP_W, float start, float stop, float radiusFunc(float,float)) {
    for (float y=start; y<stop; y+=STRIP_H) {
        for (float r=0.0; r<PI2; r+=STRIP_W) {
            pushTrianglePositionNormal( mesh,
                vec4(std::sin(r)*radiusFunc(y,r), y, std::cos(r)*radiusFunc(y,r), 1.0),
                vec4(std::sin(r+STRIP_W)*radiusFunc(y,r+STRIP_W), y, std::cos(r+STRIP_W)*radiusFunc(y,r+STRIP_W), 1.0),
                vec4(std::sin(r+STRIP_W)*radiusFunc(y+STRIP_H,r+STRIP_W), y+STRIP_H, std::cos(r+STRIP_W)*radiusFunc(y+STRIP_H,r+STRIP_W), 1.0)
            );
            mesh.pushTexcoord(vec2(((r)/PI2), (y+1)/2));
            mesh.pushTexcoord(vec2(((r+STRIP_W)/PI2), (y+1)/2));
            mesh.pushTexcoord(vec2(((r+STRIP_W)/PI2), (y+STRIP_H+1)/2));
                
            pushTrianglePositionNormal( mesh,
                vec4(std::sin(r+STRIP_W)*radiusFunc(y+STRIP_H,r+STRIP_W), y+STRIP_H, std::cos(r+STRIP_W)*radiusFunc(y+STRIP_H,r+STRIP_W), 1.0),
                vec4(std::sin(r)*radiusFunc(y+STRIP_H,r), y+STRIP_H, std::cos(r)*radiusFunc(y+STRIP_H,r), 1.0),
                vec4(std::sin(r)*radiusFunc(y,r), y, std::cos(r)*radiusFunc(y,r), 1.0)
            );
            mesh.pushTexcoord(vec2(((r+STRIP_W)/PI2), (y+STRIP_H+1)/2));
            mesh.pushTexcoord(vec2(((r)/PI2), (y+STRIP_H+1)/2));
            mesh.pushTexcoord(vec2(((r)/PI2), (y+1)/2));
        }
    }
}

void fillCircularMesh(Mesh& mesh, bool up, const float STRIP_W, float y, float radius) {
    vec4 a, b, c;
    for (float r=0.0; r<PI2; r+=STRIP_W) {
        if (up) {
            a = vec4(0.0f, y, 0.0f, 1.0f);
            b = vec4(std::cos(r+STRIP_W)*radius, y, std::sin(r+STRIP_W)*radius, 1.0f);
            c = vec4(std::cos(r)*radius, y, std::sin(r)*radius, 1.0f);
            mesh.pushTexcoord(vec2(0.5, 0.5));
            mesh.pushTexcoord(vec2(std::cos(r+STRIP_W)/2+1, std::sin(r+STRIP_W)/2+1));
            mesh.pushTexcoord(vec2(std::cos(r)/2+1, std::sin(r)/2+1));
        } else {
            a = vec4(0.0f, y, 0.0f, 1.0f);
            c = vec4(std::sin(r)*radius, y, std::cos(r)*radius, 1.0f);
            b = vec4(std::sin(r+STRIP_W)*radius, y, std::cos(r+STRIP_W)*radius, 1.0f);
            mesh.pushTexcoord(vec2(0.5, 0.5));
            mesh.pushTexcoord(vec2(std::cos(r)/2+1, std::sin(r)/2+1));
            mesh.pushTexcoord(vec2(std::cos(r+STRIP_W)/2+1, std::sin(r+STRIP_W)/2+1));
        }
        pushTrianglePositionNormal(mesh, a, b, c);
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
