#include "sync.hpp"
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include "demo_consts.hpp"

Sync::Sync(std::string filename) {
    std::ifstream file(filename.c_str());
    std::vector<std::string> lines;
    
    if (!file.is_open()) {
        std::cout << "Sync: " << filename << ": no such file.\n";
        exit(ERR_SYNC);
    }
    
    char buf[255];
    while (!file.eof()){
        file.getline(buf, 255);
        lines.push_back(std::string(buf));
    }
    
    float x, y, px=0.0f;
    char mode;
    
    for (unsigned int i=0; i<lines.size(); i++) {
        if (lines[i].c_str()[0] == '#') {
            continue;
        } else if (lines[i].c_str()[0] == 't' && lines[i].c_str()[1] == ' ') {
            mode = 'p';
            sscanf(lines[i].c_str(), "t %c", &mode);
            if (mode == 'p') {
                tracks.push_back(SyncTrack(ST_LAST_SAMPLE));
            } else if (mode == 'l') {
                tracks.push_back(SyncTrack(ST_LINEAR));
            } else if (mode == 's') {
                tracks.push_back(SyncTrack(ST_SMOOTH));
            } else {
                std::cout << "Sync: line " << i << ": \"" << lines[i] << "\" is not a valid track\n";
                exit(ERR_SYNC);
            }
        } else if (lines[i].c_str()[0] == 's' && lines[i].c_str()[1] == ' ') {
            if (tracks.size() != 0) {
                sscanf(lines[i].c_str(), "s %f %f", &x, &y);
                if (tracks.back().getNumSamples() != 0) {
                    if (x < px) {
                        std::cout << "Sync: line " << i << ": Sample time lower than previous. Samples have to be in ascending time order.\n";
                        exit(ERR_SYNC);
                    }
                }
                tracks.back().pushSample(vec2(x, y));
                px = x;
            } else {
                std::cout << "Sync: line " << i << ": Skipping sample because no track previously specified.\n";
            }
        } else {
            if (lines[i].length())
                std::cout << "Sync: line " << i << ": Unrecognized content \"" << lines[i] << "\".\n";
        }
    }
}

float Sync::getValue(unsigned int track, float t) {
    if (track < tracks.size())
        return tracks[track].getValue(t);
    return 0.0f;
}

vec2 Sync::getValue(unsigned int track, unsigned int track2, float t) {
	if (track < tracks.size() && track2 < tracks.size())
		return vec2(tracks[track].getValue(t), tracks[track2].getValue(t));
	return vec2(0.0f);
}

vec3 Sync::getValue(unsigned int track, unsigned int track2, unsigned int track3, float t) {
	if (track < tracks.size() && track2 < tracks.size() && track3 < tracks.size())
		return vec3(tracks[track].getValue(t), tracks[track2].getValue(t), tracks[track3].getValue(t));
	return vec3(0.0f);
}

vec4 Sync::getValue(unsigned int track, unsigned int track2, unsigned int track3, unsigned int track4, float t) {
	if (track < tracks.size() && track2 < tracks.size() && track3 < tracks.size() && track4 < tracks.size())
		return vec4(tracks[track].getValue(t), tracks[track2].getValue(t), tracks[track3].getValue(t), tracks[track4].getValue(t));
	return vec4(0.0f);
}

float Sync::getTime(unsigned int track, unsigned int sample) {
    if (track < tracks.size())
        return tracks[track].getTime(sample);
    return 0.0f;
}
