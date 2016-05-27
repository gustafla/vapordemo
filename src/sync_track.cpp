#include "sync_track.hpp"
#include "3dapp_gfx_utils.hpp"
#include <algorithm>

SyncTrack::SyncTrack(SyncTrackInterpolationMode _interpolationMode):
interpolationMode(_interpolationMode) {
    
}

void SyncTrack::pushSample(vec2 sample) {
    samples.push_back(sample);
}

float SyncTrack::getValue(float t) {
    if (samples.size()) {
        if (t<samples[0].x)
            return samples[0].y;
        if (t>samples.back().x)
            return samples.back().y;
        
        //TODO: Binary search could be applied here?
        int i;
        float nextt;
        float prevt;
        for(i=0; i<samples.size(); i++) {
            nextt = samples[i].x;
            if (t<=nextt)
                break;
        }
        prevt = samples[i-1].x;
        
        float prev = samples[i-1].y, next = samples[i].y;
        switch (interpolationMode) {
            case ST_LAST_SAMPLE:
                return prev;
            case ST_LINEAR:
                return ((next-prev)/(nextt-prevt))*(t-prevt)+prev;
            case ST_SMOOTH:
                return smoothstep(prevt, nextt, t)*(next-prev)+prev;
        }
    }
    
    return 0.0f;
}

unsigned int SyncTrack::getNumSamples() {
    return samples.size();
}

float SyncTrack::getTime(unsigned int sample) {
    if (sample < samples.size())
        return samples[sample].x;
    return 0.0f;
}

bool sampleCompare(const vec2& p1, const vec2& p2) {
    return (p1.x < p2.x);
}

void SyncTrack::sort() {
    std::sort(samples.begin(), samples.end(), sampleCompare);
}
