#pragma once
#include "demo_consts.hpp"

class DemoPart {
    public:
        DemoPart(float t, float _bps=DEMO_BPS);
        virtual void draw();
    
    protected:
        float start;
        float bps;
};
