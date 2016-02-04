#include "demo.hpp"
#include "graphics.hpp"
#include <iostream>
#include "part_pascal_triangles.hpp"
#include "paths.hpp"
#include "shader.hpp"
#include "geo_primitives.hpp"
#include "vectors.hpp"
#include <string>
#include <cstdio>

void setTextureUniforms(Program& shader, unsigned int n) {
    shader.use();
    char buf[16];
    for(GLuint i=0; i<n; i++) {
        sprintf(buf, "texture%d", i);
        glUniform1i(shader.getUfmHandle(std::string(buf)), i);
    }
}

Demo* Demo::instance;

Demo::Demo(Window& _window):
Application(_window),
rect(vec2(DEMO_W, DEMO_H), vec2(_window.getWidth(), _window.getHeight())),
internalRes(DEMO_W, DEMO_H),
internalAspectRatio(DEMO_W/DEMO_H),
sync("sync.txt"),
shaderPostAnalog(shaderPath("simple.vert"), shaderPath("analog.frag")),
shaderPostBlur(shaderPath("simple.vert"), shaderPath("fastblur.frag")),
shaderSimple(shaderPath("simple.vert"), shaderPath("generic.frag")),
fboPostAnalog(DEMO_W, DEMO_H),
fboPostBlur(DEMO_W, DEMO_H),
fboMain(DEMO_W, DEMO_H) {
    //glEnable(GL_CULL_FACE);
    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    check();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    check();
    
    setTextureUniforms(shaderPostAnalog);
    setTextureUniforms(shaderSimple);
    setTextureUniforms(shaderPostBlur);
    shaderPostBlur.use();
    glUniform2f(shaderPostBlur.getUfmHandle("resolution"), internalRes.x, internalRes.y);
    
    parts.push_back(new PartPascalTriangles());
}

Demo::~Demo() {
    for (unsigned int i=0; i<parts.size(); i++) {
        delete parts[i];
    }
}

void Demo::draw() {
    fboPostAnalog.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    parts[(unsigned int)(sync.getValue(SYNC_PART, window.getTime())+0.5f)]->draw();
    
    //Blur to Analog
    fboPostBlur.bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    shaderPostBlur.use();
    fboPostAnalog.getTexture().bindToUnit(0);
    GeoPrimitives::singleton().quad.draw(shaderPostBlur);
    
    //Analog postproc to scaling buffer
    fboMain.bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    shaderPostAnalog.use();
    glUniform1f(shaderPostAnalog.getUfmHandle("time"), window.getTime());
    glUniform1f(shaderPostAnalog.getUfmHandle("brightness"), sync.getValue(SYNC_BRIGHTNESS, window.getTime()));
    std::cout << sync.getValue(SYNC_BRIGHTNESS, window.getTime()) << std::endl;
    fboPostAnalog.getTexture().bindToUnit(0);
    fboPostBlur.getTexture().bindToUnit(1);
    GeoPrimitives::singleton().quad.draw(shaderPostAnalog);
    
    //Scaling buffer to window
    window.bindFramebuffer();
    glClear(GL_DEPTH_BUFFER_BIT);
    shaderSimple.use();
    fboMain.getTexture().bindToUnit(0);
    rect.draw(shaderSimple);
    
    window.swapBuffers();
    check();
}

Window& Demo::getWindow() {
    return window;
}

void Demo::createSingleton(Window& _window) {
    if (!instance)
        instance = new Demo(_window);
    else
        std::cout << "Cannot abandon initialized Demo instance.\n";
}

void Demo::destroySingleton() {
    if (instance)
        delete instance;
    else
        std::cout << "Cannot delete uninitialized Demo instance.\n";
}

Demo& Demo::singleton() {
    if (instance)
        return *instance;
    else
        std::cout << "Cannot give uninitialized Demo instance.\n";
}

float Demo::getInternalAspectRatio() {
    return internalAspectRatio;
}

vec2 Demo::getInternalResolution() {
    return internalRes;
}

Sync& Demo::getSync() {
    return sync;
}
