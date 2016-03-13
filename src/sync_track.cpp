#include "sync_track.hpp"
#include "gfx_utils.hpp"

SyncTrack::SyncTrack(SyncTrackInterpolationMode _interpolationMode):
interpolationMode(_interpolationMode) {
    
}

void SyncTrack::pushSample(vec2 sample) {
    samples.push_back(sample);
}

float SyncTrack::getValue(float t) {
    if (samples.size()) { //ONLY FOR SAMPLES IN ASCENDING TIME ORDER!! (use sorting algorithm before if needed)
        if (t<samples[0].x)
            return samples[0].y;
        if (t>samples.back().x)
            return samples.back().y;
        
        int i;
        float nextt;
        float prevt;
        for(i=0; i<samples.size(); i++) {
            nextt = samples[i].x;
            if (t<=nextt)
                break; //Sanic fast ;)
        }
        prevt = samples[i-1].x;
        //TODO: nan issues here
        float prev = samples[i-1].y, next = samples[i].y;
        switch (interpolationMode) {
            case ST_LAST_SAMPLE:
                return prev;
            case ST_LINEAR:
                return ((next-prev)/(nextt-prevt))*(t-prevt)+prev;
            case ST_SMOOTH:
                return smoothstep(prevt, nextt, t)*(next-prev)+prev;
        }
    } else {
        return 0.0f;
    }
    
    return 0.0f; //Just in case :p
}

unsigned int SyncTrack::getNumSamples() {
    return samples.size();
}

float SyncTrack::getTime(unsigned int sample) {
    if (sample < samples.size())
        return samples[sample].x;
    return 0.0f;
}
