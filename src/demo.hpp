#pragma once

#include "application.hpp"
#include "program.hpp"
#include "framebuffer.hpp"
#include "vectors.hpp"
#include <iostream>
#include <vector>
#include "demo_part.hpp"
#include "scaling_rectangle.hpp"
#include "sync.hpp"
#include "demo_consts.hpp"

class Demo: public Application {
    private:
        Demo(Window& _window);
        ~Demo();
        static Demo* instance;
        
    public:
        virtual void draw();
        
        static void createSingleton(Window& _window);
        static void destroySingleton();
        static Demo& singleton();
        
        Window& getWindow();
        Sync& getSync();
        float getInternalAspectRatio();
        vec2 getInternalResolution();
        float getTime(float scale=1.0, float offset=0.0);
        void startTimer(float t=0.0f);
        
    protected:
        std::vector<DemoPart*> parts;
        
        Sync sync;
        
        Program shaderPostAnalog;
        Program shaderPostBlur;
        Program shaderSimple;
        //Texture texturePostAnalogNoise;
        Framebuffer fboPostAnalog;
        Framebuffer fboPostBlur;
        Framebuffer fboMain;
        
        vec2 internalRes;
        float internalAspectRatio;
        ScalingRectangle rect;
        float demoStart;
};
