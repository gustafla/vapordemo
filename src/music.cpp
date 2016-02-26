#include "music.hpp"
#include "graphics.hpp"
#include <cstdlib>
#include <iostream>

void MusicPlayer::fillBuffer(uint8_t* stream, int len) {
    /* Only play if we have data left */
    if ( audioLen == 0 )
        return;

    /* Mix as much data as possible */
    len = ( len > audioLen ? audioLen : len );
    SDL_memcpy(stream, audioPos, len);
    //SDL_MixAudio(stream, audioPos, len, SDL_MIX_MAXVOLUME);
    audioPos += len;
    audioLen -= len;
    
    //std::cout << "SDL audio callback running\n";
}

MusicPlayer::MusicPlayer(std::string filename, void wrapper(void*, uint8_t*, int)) {
    std::cout << "Setting MusicPlayer for " << filename << "\n";
    
    if (SDL_LoadWAV(filename.c_str(), &wavSpec, &wavBuffer, &wavLen) == NULL){
        std::cout << "Can't read " << filename << "\n";
        exit(ERR_AUDIO);
    }
    
    wavSpec.callback = wrapper;
    wavSpec.samples = 2048;
    wavSpec.userdata = NULL;
    
    #ifdef RASPI_BUILD
        if (SDL_OpenAudio(&wavSpec, NULL) < 0) {
            std::cout << "Couldn't get audio device.\n";
            exit(ERR_AUDIO);
        }
    #else //Because the legacy method is getting buggy!
        SDL_AudioSpec have;
        audioDevice = SDL_OpenAudioDevice(NULL, 0, &wavSpec, &have, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
        if (audioDevice == 0) {
            std::cout << "Couldn't get audio device.\n";
            exit(ERR_AUDIO);
        }
        
        if (wavSpec.format != have.format) { // we let this one thing change.
            std::cout << "Couldn't get correct format for " << filename << "\n";
        }
    #endif
    
    audioPos = wavBuffer;
    audioLen = wavLen;
}

MusicPlayer::~MusicPlayer() {
    #ifdef RASPI_BUILD
        SDL_CloseAudio();
    #else
        SDL_CloseAudioDevice(audioDevice);
    #endif
    SDL_FreeWAV(wavBuffer);
}

void MusicPlayer::startPlayback() {
    #ifdef RASPI_BUILD
        SDL_PauseAudio(0);
    #else
        SDL_PauseAudioDevice(audioDevice, 0);
    #endif
    std::cout << "MusicPlayer started.\n";
}
