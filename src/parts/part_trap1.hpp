#pragma once

#include "demo.hpp"
#include "program.hpp"
#include "texture.hpp"
#include "mvp.hpp"
#include "demo_part.hpp"
#include "static_model.hpp"
#include "lighting_state.hpp"

class PartTrap1: public DemoPart {
    public:
        PartTrap1(float t, float _bps);
        ~PartTrap1();
        virtual void draw();
        
    private:
};
