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
        if (lines[i].c_str()[0] == '#' || lines[i].c_str()[0] == '\n') {
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
            std::cout << "Sync: line " << i << ": Unrecognized content \"" << lines[i] << "\".\n";
        }
    }
}

float Sync::getValue(unsigned int track, float t) {
    if (track < tracks.size())
        return tracks[track].getValue(t);
    return 0.0f;
}
