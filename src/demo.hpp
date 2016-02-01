#pragma once

#include "application.hpp"
#include "program.hpp"
#include "mvp.hpp"
#include "framebuffer.hpp"
#include <iostream>
#include <vector>
#include "demo_drawable.hpp"

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
};
