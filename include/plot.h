#pragma once

#include <limits>
#include "render.h"
#include "dynarray.h"

const sf::Color SAFETY_ORANGE = sf::Color(255, 103, 0);

class Plot: public Renderable {
private:
    dynarray<double> _measurments;
    double _max = std::numeric_limits<double>::min();
    double _min = std::numeric_limits<double>::max();
    uint _index = 0;
    uint _capacity;
    sf::Color _color;

public:
    Plot(uint capacity, const sf::Color& color = SAFETY_ORANGE)
        : _measurments(dynarray<double>(capacity, 0.0))
        , _capacity(capacity)
        , _color(color)
        {}

    void add_point(double val);
    void render(sf::RenderTexture& window) const final;
};