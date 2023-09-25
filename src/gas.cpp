#include "gas.h"
#include <cassert>
#include <cstring>

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

static bool wall_collision(BaseMolecule* molec, const Interval& x_limits, const Interval& y_limits);
static bool is_hit(BaseMolecule *lhs, BaseMolecule* rhs);
static void piston_collision(BaseMolecule* molec, double piston_y);
static void collide_nya_nya(BaseMolecule* lhs, BaseMolecule *rhs, Gas& gas);
static void collide_nya_meow(BaseMolecule* lhs, BaseMolecule *rhs, Gas& gas);
static void collide_meow_nya(BaseMolecule* lhs, BaseMolecule *rhs, Gas& gas);
static void collide_meow_meow(BaseMolecule* lhs, BaseMolecule *rhs, Gas& gas);

using collide_func_t = void(*)(BaseMolecule*, BaseMolecule*, Gas& gas);

const collide_func_t handlers[(uint) MoleculeType::ENUM_SIZE][(uint) MoleculeType::ENUM_SIZE] = {
    collide_nya_nya,
    collide_nya_meow,
    collide_meow_nya,
    collide_meow_meow
};

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

void Gas::tick() {
    uint current_size = _moleculas.size();
    _pressure = 0;

    for (uint i = 0; i < current_size; ++i) {
        if (_moleculas[i]->is_deleted) {continue;}

        _moleculas[i]->pos += _moleculas[i]->vel;

        _pressure += wall_collision(_moleculas[i], _x_limits, _y_limits);
        piston_collision(_moleculas[i], piston_y);

        for (uint j = i + 1; j < current_size; ++j) {
            if (_moleculas[i]->is_deleted) {break;}
            if (_moleculas[j]->is_deleted) {continue;}

            if (is_hit(_moleculas[i], _moleculas[j])) {
                collide(i, j);
            }
        }
    }

    gc_and_stats();
}

void Gas::collide(size_t i, size_t j) {
    collide_func_t handler = handlers[static_cast<uint>(_moleculas[i]->type())][static_cast<uint>(_moleculas[j]->type())];

    handler(_moleculas[i], _moleculas[j], *this);
}

void Gas::gc_and_stats() {
    double sum_temp = 0;
    memset(_counters, 0, sizeof(_counters)); // valid sizeof because it is uint[] type

    for (uint i = 0; i < _moleculas.size(); ++i) {
        if (_moleculas[i]->is_deleted) {
            std::swap(_moleculas[i], _moleculas[_moleculas.size() - 1]);
            _moleculas.pop_back();
        }

        double vel_sq = _moleculas[i]->vel.length_square();
        sum_temp += _moleculas[i]->mass * vel_sq;

        _counters[(uint) _moleculas[i]->type()]++;
    }

    _temp = sum_temp / _moleculas.size();
}

void Gas::change_temp(double delta) {
    double ratio;
    if (_temp + delta > 0) {
        ratio = sqrt((_temp + delta) / _temp);

        for (uint i = 0; i < _moleculas.size(); ++i) {
            _moleculas[i]->vel *= ratio;
        }
    } else {
        for (uint i = 0; i < _moleculas.size(); ++i) {
            _moleculas[i]->vel *= MIN_VELOCITY / _moleculas[i]->vel.length();
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

static bool wall_collision(BaseMolecule* molec, const Interval& x_limits, const Interval& y_limits) {
    bool res = false;
    if (!x_limits.contains(molec->pos.x)) {
        molec->vel.x *= -1;
        res = true;
    }

    if (!y_limits.contains(molec->pos.y)) {
        molec->vel.y *= -1;
        res = true;
    }

    return res;
}

static void piston_collision(BaseMolecule* molec, double piston_y) {
    if (molec->pos.y >= piston_y) {
        molec->vel.y *= -1;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

static bool is_hit(BaseMolecule *lhs, BaseMolecule* rhs) {
    return (lhs->pos - rhs->pos).length() <= (lhs->radius + rhs->radius);
}

// ---------------------------------------------------------------------------------------------------------------------

static void collide_nya_nya(BaseMolecule* lhs, BaseMolecule *rhs, Gas& gas) {
    assert(typeid(*lhs) == typeid(NyaMolec) && "Incorrect lhs type");
    assert(typeid(*rhs) == typeid(NyaMolec) && "Incorrect rhs type");

    double total_mass = (lhs->mass + rhs->mass);

    Vector new_vel = (lhs->vel * lhs->mass + rhs->vel * rhs->mass) / total_mass;
    Point new_pos = lhs->pos + rhs->mass/total_mass * (rhs->pos - lhs->pos);

    gas.add(new MeowMolec(new_pos, new_vel, total_mass));

    gas.mark_deleted(lhs);
    gas.mark_deleted(rhs);
}

static void collide_nya_meow(BaseMolecule* lhs, BaseMolecule *rhs, Gas& gas) {
    assert(typeid(*lhs) == typeid(NyaMolec) && "Incorrect lhs type");
    assert(typeid(*rhs) == typeid(MeowMolec) && "Incorrect rhs type");

    double total_mass = (lhs->mass + rhs->mass);

    rhs->vel = (lhs->vel * lhs->mass + rhs->vel * rhs->mass) / total_mass;
    rhs->mass = total_mass;
    rhs->radius = BASE_RADIUS + rhs->mass * MASS_RADIUS_COEFF;


    gas.mark_deleted(lhs);
}

static void collide_meow_nya(BaseMolecule* lhs, BaseMolecule *rhs, Gas& gas) {
    assert(typeid(*lhs) == typeid(MeowMolec) && "Incorrect lhs type");
    assert(typeid(*rhs) == typeid(NyaMolec) && "Incorrect rhs type");

    collide_nya_meow(rhs, lhs, gas);
}

static void collide_meow_meow(BaseMolecule* lhs, BaseMolecule *rhs, Gas& gas) {
    assert(typeid(*lhs) == typeid(MeowMolec) && "Incorrect lhs type");
    assert(typeid(*rhs) == typeid(MeowMolec) && "Incorrect rhs type");

    uint total_mass = lhs->mass + rhs->mass;
    Point avg_pos = lhs->pos + rhs->mass/total_mass * (rhs->pos - lhs->pos);
    double avg_vel = ((lhs->vel * lhs->mass + rhs->vel * rhs->mass) / total_mass).length();

    for (uint i = 0; i < total_mass; ++i) {
        Vector direction = Vector::random(-1, 1).norm();
        Point p_pos = avg_pos + direction * BASE_RADIUS * 4;
        gas.spawn_random<NyaMolec>(p_pos);
    }

    gas.mark_deleted(lhs);
    gas.mark_deleted(rhs);
}
