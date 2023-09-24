#pragma once

#include <memory>
#include "point.h"
#include "vector.h"
#include "dynarray.h"
#include "config.h"
#include "interval.h"

class BaseMolecule {
public:
    Point pos;
    Vector vel;
    double mass;

    double radius;

    BaseMolecule(Point pos_, Vector vel_, double mass_):
        pos(pos_),
        vel(vel_),
        mass(mass_),
        radius(BASE_RADIUS + mass_ * MASS_RADIUS_COEFF)
        {};

    BaseMolecule(const BaseMolecule&) = default;
    BaseMolecule(BaseMolecule&&) = default;

};

class NyaMolec: public BaseMolecule {
public:
    NyaMolec(Point pos_, Vector vel_, double mass_):
        BaseMolecule(pos_, vel_, mass_)
        {};
};

class MeowMolec: public BaseMolecule {
public:
    MeowMolec(Point pos_, Vector vel_, double mass_):
        BaseMolecule(pos_, vel_, mass_)
        {};
};

class Gas {
private:
    dynarray<BaseMolecule *> _moleculas;
    Interval _x_limits;
    Interval _y_limits;

public:
    Gas() = default;

    Gas(Interval x_limits, Interval y_limits):
        _x_limits(x_limits),
        _y_limits(y_limits)
        {};

    void add(BaseMolecule *mol) {
        _moleculas.push_back(mol);
    }

    void tick();

    const dynarray<BaseMolecule *>& moleculas() const noexcept { return _moleculas; }

    ~Gas() {
        for (uint i = 0; i < _moleculas.size(); ++i) {
            delete _moleculas[i];
        }
    }
};