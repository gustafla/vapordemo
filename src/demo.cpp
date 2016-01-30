#include "demo.hpp"
#include "graphics.hpp"
#include "vectors.hpp"
#include "obj_file.hpp"
#include <cmath>
#include <iostream>
#include <cmath>
#include "geo_primitives.hpp"

Demo::Demo(Window& _window):
Application(_window),
ppBuf(_window.getWidth(), _window.getHeight()) {
    Shader vertexShader(shaderPath("mvptex.vert"));
    Shader fragmentShader(shaderPath("showtex_var.frag"));
    shaderProgram = new Program(vertexShader, fragmentShader);
    
    Shader ppvertexShader(shaderPath("simple.vert"));
    Shader ppfragmentShader(shaderPath("wave.frag"));
    ppProgram = new Program(ppvertexShader, ppfragmentShader);
    
    projection = getPProjMat(45.0f, window.getAspect());
    mvp = new MVP(projection, 0.0, 0.0, -5.0f);
    shaderProgram->use();
    check();
    glUniform1i(shaderProgram->getUfmHandle("iChannel0"), 0);
    check();
    //glEnable(GL_CULL_FACE);
    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    check();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    check();
    
    ppProgram->use();
    glUniform1i(ppProgram->getUfmHandle("iChannel0"), 0);
    glUniform2f(ppProgram->getUfmHandle("iResolution"), window.getWidth(), window.getHeight());
    glUniform1f(ppProgram->getUfmHandle("iGlobalTime"), window.getTime());
}

void Demo::draw() {
    resources.getTexture("test.tga")->bindToUnit(0);
    check();
    projection = getPProjMat(45.0, window.getAspect());
    ppBuf.bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    check();
    glClearColor(sin(window.getTime()*M_PI)*0.5+0.5, 0.0, 0.0, 1.0);
    check();
    shaderProgram->use();
    check();
    //glUniform4f(shader.getUfmHandle("u_color"), 1.0, 1.0, 0.5, 1.0);
    mvp->setModelRotation(0.0, window.getTime()/2.0, window.getTime());
    mvp->buildModel();
    mvp->buildMVP();
    glUniformMatrix4fv(shaderProgram->getUfmHandle("mvp"), 1, GL_FALSE, mvp->getMVPArray());
    check();
    resources.getStaticModel("cube_tex.obj")->draw(*shaderProgram);
    check();
    
    window.bindFramebuffer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ppProgram->use();
    glUniform1f(ppProgram->getUfmHandle("iGlobalTime"), window.getTime());
    ppBuf.getTexture().bindToUnit(0);
    GeoPrimitives::singleton().quad.draw(*ppProgram);
    
    window.swapBuffers();
    check();
}
