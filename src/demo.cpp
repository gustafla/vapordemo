#include "demo.hpp"
#include "graphics.hpp"
#include <iostream>
#include "part_pascal_triangles.hpp"

Demo* Demo::instance;

Demo::Demo(Window& _window):
currentPart(0),
Application(_window) {
    //glEnable(GL_CULL_FACE);
    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    check();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    check();
    
    parts.push_back(PartPascalTriangles());
}

void Demo::draw() {
    window.bindFramebuffer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    parts[currentPart].draw();
        
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
