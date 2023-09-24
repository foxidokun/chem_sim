#include "common.h"
#include "render.h"

void render(sf::RenderWindow& window, const Gas& gas) {
    const dynarray<BaseMolecule *> moleculas = gas.moleculas();

    for (uint i = 0; i < moleculas.size(); ++i) {
        sf::CircleShape circle;
        circle.setRadius(10);
        circle.setFillColor(sf::Color::Red);
        circle.setPosition(moleculas[i]->pos.x, moleculas[i]->pos.y);
        window.draw(circle);
    }
}