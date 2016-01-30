#include <iostream>
#include "window.hpp"
#include "config.hpp"
#include "demo.hpp"
#include "vectors.hpp"
#include <vector>
#include <cstdlib>
#include "graphics.hpp"

void cleanup() {
    #ifdef RASPI_BUILD
        bcm_host_deinit();
    #else
        SDL_Quit();
    #endif
}

int main(int argc, char* argv[]) {
    #ifdef RASPI_BUILD
        bcm_host_init();
    #endif
    const Config conf(argc, argv);
    Window window(conf);
    atexit(cleanup);
    Demo demo(window);

    float frames=0;
    float tLast=0;
    const float TIME=5.0f;

    while(demo.isRunning()) {
        demo.draw();
        
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
