#pragma once

#include "3dapp_vectors.hpp"
#include <vector>

typedef enum _SyncTrackInterpolationMode {
    ST_LINEAR,
    ST_SMOOTH,
    ST_LAST_SAMPLE
} SyncTrackInterpolationMode;

class SyncTrack {
    public:
        SyncTrack(SyncTrackInterpolationMode _interpolationMode=ST_LINEAR);
        void pushSample(vec2 sample);
        float getValue(float t);
        float getTime(unsigned int sample);
        unsigned int getNumSamples();
        void sort();
    private:
        SyncTrackInterpolationMode interpolationMode;
        std::vector<vec2> samples;
};
