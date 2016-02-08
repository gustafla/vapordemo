#pragma once

#include <vector>
#include <string>
#include "sync_track.hpp"
#include "vectors.hpp"

class Sync {
    public:
        Sync(std::string filename);
        float getValue(unsigned int track, float t);
        vec2  getValue(unsigned int track, unsigned int track2, float t);
        vec3  getValue(unsigned int track, unsigned int track2, unsigned int track3, float t);
        vec4  getValue(unsigned int track, unsigned int track2, unsigned int track3, unsigned int track4, float t);
        float getTime(unsigned int track, unsigned int sample);

    private:
        std::vector<SyncTrack> tracks;
};
