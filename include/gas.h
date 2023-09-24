#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "point.h"
#include "vector.h"
#include "dynarray.h"
#include "config.h"
#include "interval.h"
#include <cassert>

// Not-extensible solution
enum class MoleculeType {
    NyaMolec  = 0,
    MeowMolec = 1,
    ENUM_SIZE
};

class BaseMolecule {
protected:
    MoleculeType _type;
public:
    Point pos;
    Vector vel;
    double mass;
    double radius;

    bool is_deleted = false;

    BaseMolecule(Point pos_, Vector vel_, double mass_):
        pos(pos_),
        vel(vel_),
        mass(mass_),
        radius(BASE_RADIUS + mass_ * MASS_RADIUS_COEFF)
        {};

    BaseMolecule(const BaseMolecule&) = default;
    BaseMolecule(BaseMolecule&&)      = default;

    MoleculeType type() const noexcept { return _type; };

    virtual void draw(sf::RenderWindow& window) const = 0;

    virtual ~BaseMolecule() = default;
};

class NyaMolec: public BaseMolecule {
public:
    NyaMolec(Point pos_, Vector vel_, double mass_):
        BaseMolecule(pos_, vel_, mass_)
        {
            _type = MoleculeType::NyaMolec;
        };
    
    void draw(sf::RenderWindow& window) const final;
};

class MeowMolec: public BaseMolecule {
public:
    MeowMolec(Point pos_, Vector vel_, double mass_):
        BaseMolecule(pos_, vel_, mass_)
        {
            _type = MoleculeType::MeowMolec;
        };
    
    void draw(sf::RenderWindow& window) const final;
};

class Gas {
private:
    dynarray<BaseMolecule *> _moleculas;
    Interval _x_limits;
    Interval _y_limits;

    void collide(size_t i, size_t j);

    uint _gc_count;
public:
    Gas() = default;

    Gas(Interval x_limits, Interval y_limits):
        _x_limits(x_limits),
        _y_limits(y_limits)
        {};

    void add(BaseMolecule *mol) {
        _moleculas.push_back(mol);
    }

    template<typename T>
    void spawn_random();

    void tick();

    void mark_deleted(BaseMolecule *mol) {
        assert(!mol->is_deleted);

        ++_gc_count;
        mol->is_deleted = true;
    }

    const dynarray<BaseMolecule *>& moleculas() const noexcept { return _moleculas; }

    ~Gas() {
        for (uint i = 0; i < _moleculas.size(); ++i) {
            delete _moleculas[i];
        }
    }
};

template<typename T>
void Gas::spawn_random() {
    Point pos = Point(random_double(_x_limits.min, _x_limits.max), random_double(_y_limits.min, _y_limits.max), 0);

    // TODO velocity based on gas temperature
    add(new T(pos, Vector::random(-0.1, 0.1), 10));
};
