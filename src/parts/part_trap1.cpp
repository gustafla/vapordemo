#include "parts/part_trap1.hpp"
#include "demo.hpp"

PartTrap1::PartTrap1(float t, float _bps):
DemoPart(t, _bps) {
}

void PartTrap1::draw() {
    Sync& sync = Demo::singleton().getSync();
}
