#include <iostream>
#include "window.hpp"
#include "config.hpp"
#include "demo.hpp"
#include "vectors.hpp"
#include <vector>
#include <cstdlib>
#include "graphics.hpp"

void cleanup() {
    Demo::destroySingleton();
    cleanupGraphics();
}

int main(int argc, char* argv[]) {
    initializeGraphics();
    
    const Config conf(argc, argv);
    Window window(conf, "Revision demo test");
    Demo::createSingleton(window);
    atexit(cleanup);

    float frames=0;
    float tLast=0;
    const float TIME=5.0f;

    while(Demo::singleton().isRunning()) {
        Demo::singleton().draw();
        
        frames+=1;
        if (tLast+TIME < window.getTime()) {
            std::cout << "FPS: " << (frames/TIME) << std::endl;
            frames=0;
            tLast=window.getTime();
        }
        
        check();
    }

    return 0;
}
