#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "point.h"
#include "vector.h"
#include "dynarray.h"
#include "config.h"
#include "interval.h"
#include "render.h"
#include <cassert>

// ---------------------------------------------------------------------------------------------------------------------
// Molecula types
// ---------------------------------------------------------------------------------------------------------------------

// Not-extensible solution
enum class MoleculeType {
    NyaMolec  = 0,
    MeowMolec = 1,
    ENUM_SIZE
};

class BaseMolecule: public Renderable {
protected:
    MoleculeType _type;
public:
    Point pos;
    Vector vel;
    uint mass;
    double radius;
    double pot_energy = 0.0;

    bool is_deleted = false;

    BaseMolecule(Point pos_, Vector vel_, uint mass_):
        pos(pos_),
        vel(vel_),
        mass(mass_),
        radius(BASE_RADIUS + mass_ * MASS_RADIUS_COEFF)
        {};

    BaseMolecule(const BaseMolecule&) = default;
    BaseMolecule(BaseMolecule&&)      = default;

    MoleculeType type() const noexcept { return _type; };
    double energy() { return mass * vel.length_square(); }

    virtual ~BaseMolecule() = default;
};

class NyaMolec: public BaseMolecule {
public:
    NyaMolec(Point pos_, Vector vel_, uint mass_):
        BaseMolecule(pos_, vel_, mass_)
        {
            _type = MoleculeType::NyaMolec;
        };
    
    void render(sf::RenderTexture& window) const final;
};

class MeowMolec: public BaseMolecule {
public:
    MeowMolec(Point pos_, Vector vel_, uint mass_):
        BaseMolecule(pos_, vel_, mass_)
        {
            _type = MoleculeType::MeowMolec;
        };
    
    void render(sf::RenderTexture& window) const final;
};

// ---------------------------------------------------------------------------------------------------------------------
// Gas as molecula holder
// ---------------------------------------------------------------------------------------------------------------------

class Gas: public Renderable {
private:
    dynarray<BaseMolecule *> _moleculas;
    Interval _x_limits;
    Interval _y_limits;

    double _temp;
    uint _counters[(uint) MoleculeType::ENUM_SIZE];
    uint _pressure;

    void collide(size_t i, size_t j);
    void gc_and_stats();

public:
    double piston_y;

    Gas() = default;

    Gas(Interval x_limits, Interval y_limits):
        _x_limits(x_limits),
        _y_limits(y_limits)
        {};

    void add(BaseMolecule *mol) {
        mol->pos.x = _x_limits.clamp(mol->pos.x);
        mol->pos.y = _x_limits.clamp(mol->pos.y);
        mol->pos.y = std::min(mol->pos.y, piston_y);

        _moleculas.push_back(mol);
    }

    template<typename T>
    void spawn_random();

    template<typename T>
    void spawn_random(Point pos);

    void tick();
    void render(sf::RenderTexture& window) const final;
    void change_temp(double delta);

    void mark_deleted(BaseMolecule *mol) {
        assert(!mol->is_deleted);
        mol->is_deleted = true;
    }

    double temp() const noexcept { return _temp; }
    uint pressure() const noexcept { return _pressure; }
    const uint *counters() const noexcept { return _counters; }
    const dynarray<BaseMolecule *>& moleculas() const noexcept { return _moleculas; }

    ~Gas() {
        for (uint i = 0; i < _moleculas.size(); ++i) {
            delete _moleculas[i];
        }
    }
};

template<typename T>
void Gas::spawn_random() {
    Point pos = Point(random_double(_x_limits.min, _x_limits.max),
                        random_double(_y_limits.min, std::min(_y_limits.max, piston_y)));

    spawn_random<T>(pos);
};

template<typename T>
void Gas::spawn_random(Point pos) {
    Vector vel_direction = Vector::random(-1, 1).norm();
    double vel_val = std::max(sqrt(_temp), MIN_VELOCITY);
    add(new T(pos, vel_val * vel_direction, 1));
};
