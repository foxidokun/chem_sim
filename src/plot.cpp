#include "plot.h"
#include <cassert>
#include <cmath>

#define WRAP_I(val) ((val < _capacity) ? val : 0) 

void Plot::add_point(double val) {
    double prev_val = _measurments[_index];

    _measurments[_index] = val;
    ++_index;
    assert (_index <= _capacity);
    _index = (_index < _capacity) ? _index : 0;

    bool need_limits_rescan = false;

    if (prev_val == _max && prev_val > val) {
        need_limits_rescan = true;
    } else {
        _max = std::max(_max, val);
    }

    if (prev_val == _min && prev_val < val) {
        need_limits_rescan = true;
    } else {
        _min = std::min(_min, val);
    }

    if (need_limits_rescan) {
        _max = std::numeric_limits<double>::min();
        _min = std::numeric_limits<double>::max();

        for (uint i = 0; i < _capacity; ++i) {
            _max = std::max(_max, _measurments[i]);
            _min = std::min(_max, _measurments[i]);
        }
    }
}

#include <iostream>
void Plot::render(sf::RenderTexture& window) const {
    auto texture_size = window.getSize();

    double new_max = std::pow(10, std::ceil (std::log10(std::fabs(_max))));
    double new_min = std::pow(10, std::floor(std::log10(std::fabs(_max))));

    double x_step = ((double)texture_size.x) / (_capacity + 1);
    double y_step = 0.96 * texture_size.y / (new_max - new_min);

    double x_gap = x_step / 2;
    double y_gap = 0.02 * texture_size.y;

    for (uint i = WRAP_I(_index + 1), x_pos = 0; i != _index; i = WRAP_I(i+1), ++x_pos) {
        sf::RectangleShape point(sf::Vector2f(2, 2));
        point.setFillColor(_color);
        point.setRotation(45);
        
        point.setPosition(x_gap + x_step * x_pos, y_gap + (_measurments[i] - new_min) * y_step);
        window.draw(point);
    }
}