#pragma once

#include "config.hpp"

class DemoConfig: public Config {
    public:
        DemoConfig(int argc, char** argv);
        float t;
        bool sound;
        
    private:
        void parseArgs(int argc, char** argv);
};
