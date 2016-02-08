#include "demo_config.hpp"
#include "util.hpp"
#include <cstring>
#include <cstdlib>

DemoConfig::DemoConfig(int argc, char** argv):
Config(argc, argv),
t(0.0f) {
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
        }
    }
}
