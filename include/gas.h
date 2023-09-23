#pragma once

#include "point.h"
#include "vector.h"
#include "config.h"

class BaseMolecule {
private:
    Point pos;
    Vector vel;
    double mass;

    double radius;

public:
    BaseMolecule(Point pos_, Vector vel_, double mass_):
        pos(pos_),
        vel(vel_),
        mass(mass_),
        radius(BASE_RADIUS + mass_ * MASS_RADIUS_COEFF)
        {};
};

class NyaMolec: public BaseMolecule {};

class MeowMolec: public BaseMolecule {};

class Gas {
}