#include "demo_config.hpp"
#include "util.hpp"
#include <cstring>
#include <cstdlib>

DemoConfig::DemoConfig(int argc, char** argv):
Config(argc, argv),
t(0.0f),
sound(true) {
    int i=1;

    for (; i<argc; i++) {
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
        } else if (strcmp(argv[i], "--no-sound") || strcmp(argv[i], "--no-music")) {
            sound = false;
        }
    }
}
