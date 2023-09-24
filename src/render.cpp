#include "common.h"
#include "render.h"
#include "gas.h"

static void render_piston(sf::RenderTexture& window, double piston_y);

void render(sf::RenderTexture& window, const Gas& gas) {
    const dynarray<BaseMolecule *> moleculas = gas.moleculas();

    for (uint i = 0; i < moleculas.size(); ++i) {
        if (!moleculas[i]->is_deleted) {
            moleculas[i]->draw(window);
        }
    }

    render_piston(window, gas.piston_y);
}

void NyaMolec::draw(sf::RenderTexture& window) const {
    sf::CircleShape circle;
    circle.setRadius(radius);
    circle.setFillColor(sf::Color::Red);
    circle.setPosition(pos.x, pos.y);
    window.draw(circle);
}

void MeowMolec::draw(sf::RenderTexture& window) const {
    sf::CircleShape circle;
    circle.setRadius(radius);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(pos.x, pos.y);
    window.draw(circle);
}

static void render_piston(sf::RenderTexture& window, double piston_y) {
    sf::Vector2u window_size = window.getSize();

    sf::RectangleShape piston(sf::Vector2f(window_size.x, PISTON_HEIGHT));
    piston.setPosition(0, piston_y + PISTON_HEIGHT);
    piston.setFillColor(sf::Color::Cyan);

    window.draw(piston);
}