#include "common.h"
#include "render.h"
#include "gas.h"

void render(sf::RenderWindow& window, const Gas& gas) {
    const dynarray<BaseMolecule *> moleculas = gas.moleculas();

    for (uint i = 0; i < moleculas.size(); ++i) {
        if (!moleculas[i]->is_deleted) {
            moleculas[i]->draw(window);
        }
    }
}

void NyaMolec::draw(sf::RenderWindow& window) const {
    sf::CircleShape circle;
    circle.setRadius(radius);
    circle.setFillColor(sf::Color::Red);
    circle.setPosition(pos.x, pos.y);
    window.draw(circle);
}

void MeowMolec::draw(sf::RenderWindow& window) const {
    sf::CircleShape circle;
    circle.setRadius(radius);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(pos.x, pos.y);
    window.draw(circle);
}