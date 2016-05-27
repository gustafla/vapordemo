#pragma once

#include "demo_consts.hpp"
#include <string>
#include <cstdlib>
#include <stdint.h>
#include "3dapp_graphics.hpp"

class MusicPlayer {
    public:
        MusicPlayer(std::string filename, void wrapper(void*, uint8_t*, int), float t=0.0);
        ~MusicPlayer();
        void startPlayback();
        
        void fillBuffer(uint8_t* stream, int len);
        
    private:
        uint8_t* wavBuffer;
        uint32_t wavLen;
        uint32_t audioLen;
        uint8_t* audioPos;
        SDL_AudioSpec wavSpec;
        
        #ifndef RASPI_BUILD
            SDL_AudioDeviceID audioDevice;
        #endif
};
