#pragma once

#include <vector>
#include <string>
#include "sync_track.hpp"

class Sync {
    public:
        Sync(std::string filename);
        float getValue(unsigned int track, float t);
    private:
        std::vector<SyncTrack> tracks;
};
