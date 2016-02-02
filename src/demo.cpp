#include "demo.hpp"
#include "graphics.hpp"
#include <iostream>
#include "part_pascal_triangles.hpp"
#include "paths.hpp"
#include "shader.hpp"
#include "geo_primitives.hpp"
#include <string>
#include <stdio.h>

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
internalRes(DEMO_W, DEMO_H),
currentPart(0),
shaderPostAnalog(shaderPath("simple.vert"), shaderPath("analog.frag")),
shaderSimple(shaderPath("simple.vert"), shaderPath("generic.frag")),
fboPostAnalog(DEMO_W, DEMO_H),
fboMain(DEMO_W, DEMO_H) {
    //glEnable(GL_CULL_FACE);
    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    check();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    check();
    
    parts.push_back(new PartPascalTriangles());
    
    setTextureUniforms(shaderPostAnalog);
    setTextureUniforms(shaderSimple);
}

Demo::~Demo() {
    for (unsigned int i=0; i<parts.size(); i++) {
        delete parts[i];
    }
}

void Demo::draw() {
    fboPostAnalog.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    parts[currentPart]->draw();
    
    //Postproc to scaling buffer
    fboMain.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shaderPostAnalog.use();
    glUniform1f(shaderPostAnalog.getUfmHandle("time"), window.getTime());
    fboPostAnalog.getTexture().bindToUnit(0);
    GeoPrimitives::singleton().quad.draw(shaderPostAnalog);
    
    //Scaling buffer to window
    window.bindFramebuffer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shaderSimple.use();
    fboMain.getTexture().bindToUnit(0);
    GeoPrimitives::singleton().quad.draw(shaderSimple);
    
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
