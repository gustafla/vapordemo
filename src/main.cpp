#include <iostream>
#include "window.hpp"
#include "demo_config.hpp"
#include "demo.hpp"
#include "vectors.hpp"
#include <vector>
#include <cstdlib>
#include "graphics.hpp"
#include "demo_consts.hpp"
#include "define.hpp"
#include "music.hpp"

MusicPlayer* player=NULL;

void cleanup() {
    Demo::singleton().getWindow().close();
    Demo::destroySingleton();
    if (player)
        delete player;
    cleanupGraphics();
}

void musicCallbackWrapper(void* ud, uint8_t* st, int l) {
    if (player)
        player->fillBuffer(st, l);
}

int main(int argc, char* argv[]) {
    initializeGraphics();
    srand(42);
    
    DemoConfig conf(argc, argv);
    if (!conf.fullscreen) {
        if (conf.w == SCREEN_W)
            conf.w = DEMO_W*DEMO_POST_SIZE_MULT;
        if (conf.h == SCREEN_H)
            conf.h = DEMO_H*DEMO_POST_SIZE_MULT;
    }
    Window window(conf, "Revision demo test", vec2(DEMO_W*DEMO_POST_SIZE_MULT, DEMO_H*DEMO_POST_SIZE_MULT));
    Demo::createSingleton(window);
    atexit(cleanup);

    float frames=0;
    float tLast=0;
    const float TIME=5.0f;

    player = new MusicPlayer(DEMO_MUSIC_FILE, musicCallbackWrapper, conf.t);
    if (conf.sound)
        player->startPlayback();
	Demo::singleton().startTimer(conf.t);
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
