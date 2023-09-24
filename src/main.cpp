#include "common.h"
#include <SFML/Graphics.hpp>
#include "render.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);

    Gas gas;
    gas.add(new NyaMolec(Point(30, 30, 0), Vector(), 20));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return 0;
            }
        }

        render(window, gas);

        // display
        window.display();
    }
}