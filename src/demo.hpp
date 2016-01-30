#pragma once

#include "application.hpp"
#include "program.hpp"
#include "mvp.hpp"
#include "framebuffer.hpp"

class Demo: public Application {
    public:
        Demo(Window& _window);
        void draw();
        
    protected:
        Program* shaderProgram;
        Program* ppProgram;
        mat4 projection;
        MVP* mvp;
        Framebuffer ppBuf;
};
