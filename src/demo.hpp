#pragma once

#include "application.hpp"
#include "program.hpp"
#include "framebuffer.hpp"
#include "vectors.hpp"
#include <iostream>
#include <vector>
#include "demo_drawable.hpp"
#include "scaling_rectangle.hpp"

#define DEMO_W 320
#define DEMO_H 240

void setTextureUniforms(Program& shader, unsigned int n=4);

class Demo: public Application {
    private:
        Demo(Window& _window);
        ~Demo();
        
    public:
        virtual void draw();
        
        static void createSingleton(Window& _window);
        static void destroySingleton();
        static Demo& singleton();
        
        Window& getWindow();
        
        static Demo* instance;
        
    protected:
        std::vector<DemoDrawable*> parts;
        unsigned int currentPart;
        
        Program shaderPostAnalog;
        Program shaderSimple;
        //Texture texturePostAnalogNoise;
        Framebuffer fboPostAnalog;
        Framebuffer fboMain;
        
        vec2 internalRes;
        ScalingRectangle rect;
};
