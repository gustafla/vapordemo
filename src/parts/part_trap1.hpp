#pragma once

#include "demo.hpp"
#include "3dapp_program.hpp"
#include "3dapp_texture.hpp"
#include "3dapp_mvp.hpp"
#include "demo_part.hpp"
#include "3dapp_static_model.hpp"
#include "3dapp_lighting_state.hpp"

class PartTrap1: public DemoPart {
    public:
        PartTrap1(float t, float _bps);
        ~PartTrap1();
        virtual void draw();
        
    private:
};
