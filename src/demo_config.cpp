#include "demo_config.hpp"
#include "util.hpp"
#include <cstring>
#include <cstdlib>

DemoConfig::DemoConfig(int argc, char** argv):
Config(argc, argv),
t(0.0f),
sound(true) {
    parseArgs(argc, argv);
}

void DemoConfig::parseArgs(int argc, char** argv) {
    int i;

    for (i=1; i<argc; i++) {
        if (strcmp(argv[i], "-t") == 0) {
            i++;
            if (i < argc) {
                if (isfloat(std::string(argv[i]))) {
                    t = atof(argv[i]);
                } else {
                    argErr(argv[i-1]);
                }
            } else {
                argErr(argv[i-1]);
            }
        } else if (strcmp(argv[i], "--no-sound") == 0 || strcmp(argv[i], "--no-music") == 0) {
            sound = false;
        }
    }
}
