#include "part_vapor1.hpp"
#include "tga_file.hpp"
#include "obj_file.hpp"
#include "paths.hpp"
#include <cmath>

PartVapor1::PartVapor1(float t):
DemoPart(t),
pillarTexture(loadTGAFile(texturePath("marble.tga"))),
shader(Shader(shaderPath("generic.vert")), Shader(shaderPath("generic.frag"))),
mvp(getPProjMat(45, DEMO_W/DEMO_H, 0.1, 10.0)) {
    genPillarMesh();
    shader.use();
    setTextureUniforms(shader);
    glUniform4f(shader.getUfmHandle("color"), 0.0 , 0.0, 0.0, 0.3);
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
    mvp.setModelRotation(0.0, DEMO_T(), 0.1);
    mvp.setModelTranslation(0.8, 0.0, 0.0);
    mvp.buildModel();
    mvp.buildMVP();
    glUniformMatrix4fv(shader.getUfmHandle("mvp"), 1, GL_FALSE, mvp.getMVPArray());
    pillar->draw(shader);
    
    mvp.setModelRotation(0.0, DEMO_T(), 0.1);
    mvp.setModelTranslation(-0.8, 0.0, 0.0);
    mvp.buildModel();
    mvp.buildMVP();
    glUniformMatrix4fv(shader.getUfmHandle("mvp"), 1, GL_FALSE, mvp.getMVPArray());
    pillar->draw(shader);
    
    glDisable(GL_DEPTH_TEST);
}

float getW(float y, float r) {
    return 0.3*std::cos(y*0.2*DEMO_PI)+std::sin(r*16.0)*0.01;
}

void PartVapor1::genPillarMesh() {
    Mesh mesh;
    
    const float STRIP_H = 0.4;
    const float STRIP_W = 0.2;
    const float PI2 = 2.0*DEMO_PI;
    
    for (float y=-1.0; y<1.0; y+=STRIP_H) {
        for (float r=0.0; r<PI2; r+=STRIP_W) {
                mesh.pushPosition(vec4(std::sin(r)*getW(y,r), y, std::cos(r)*getW(y,r), 1.0));
                mesh.pushPosition(vec4(std::sin(r+STRIP_W)*getW(y,r+STRIP_W), y, std::cos(r+STRIP_W)*getW(y,r+STRIP_W), 1.0));
                mesh.pushPosition(vec4(std::sin(r+STRIP_W)*getW(y+STRIP_H,r+STRIP_W), y+STRIP_H, std::cos(r+STRIP_W)*getW(y+STRIP_H,r+STRIP_W), 1.0));
                mesh.pushTexcoord(vec2(r/PI2, (y+1)/2));
                mesh.pushTexcoord(vec2((r+STRIP_W)/PI2, (y+1)/2));
                mesh.pushTexcoord(vec2((r+STRIP_W)/PI2, (y+STRIP_H+1)/2));
                
                mesh.pushPosition(vec4(std::sin(r+STRIP_W)*getW(y+STRIP_H,r+STRIP_W), y+STRIP_H, std::cos(r+STRIP_W)*getW(y+STRIP_H,r+STRIP_W), 1.0));
                mesh.pushPosition(vec4(std::sin(r)*getW(y+STRIP_H,r), y+STRIP_H, std::cos(r)*getW(y+STRIP_H,r), 1.0));
                mesh.pushPosition(vec4(std::sin(r)*getW(y,r), y, std::cos(r)*getW(y,r), 1.0));
                mesh.pushTexcoord(vec2((r+STRIP_W)/PI2, (y+STRIP_H+1)/2));
                mesh.pushTexcoord(vec2(r/PI2, (y+STRIP_H+1)/2));
                mesh.pushTexcoord(vec2(r/PI2, (y+1)/2));
        }
    }
    
    Mesh cube = loadOBJFile(meshPath("cube_tex.obj"));
    mat4 cubeScaleMat = getScaleMat(0.32, 0.1, 0.32);
        
    for (int i=0; i < cube.getPositions().size(); i++) {
        mesh.pushPosition(cubeScaleMat * vec4(cube.getPositions()[i].x, cube.getPositions()[i].y+10.0, cube.getPositions()[i].z, cube.getPositions()[i].w));
        mesh.pushTexcoord(cube.getTexcoords()[i]);
    }

    for (int i=0; i < cube.getPositions().size(); i++) {
        mesh.pushPosition(cubeScaleMat * vec4(cube.getPositions()[i].x, cube.getPositions()[i].y-10.0, cube.getPositions()[i].z, cube.getPositions()[i].w));
        mesh.pushTexcoord(cube.getTexcoords()[i]);
    }
    
    pillar = new StaticModel(mesh);
}
