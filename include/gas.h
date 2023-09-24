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

class Gas: public Renderable {
private:
    dynarray<BaseMolecule *> _moleculas;
    Interval _x_limits;
    Interval _y_limits;
    uint _gc_count;

    void collide(size_t i, size_t j);
    void gc();

public:
    double piston_y;

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

    template<typename T>
    void spawn_random(Point pos);

    void tick();
    void render(sf::RenderTexture& window) const;

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
    Point pos = Point(random_double(_x_limits.min, _x_limits.max),
                        random_double(_y_limits.min, std::min(_y_limits.max, piston_y)));

    spawn_random<T>(pos);
};

template<typename T>
void Gas::spawn_random(Point pos) {
    // TODO velocity based on gas temperature
    add(new T(pos, Vector::random(-0.2, 0.1), 1));
};
