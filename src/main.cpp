#include "common.h"
#include <SFML/Graphics.hpp>
#include "render.h"
#include <chrono>
#include <thread>

namespace chrono = std::chrono;

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);

    Gas gas(Interval(0, WINDOW_WIDTH), Interval(0, WINDOW_HEIGHT));
    gas.piston_y = WINDOW_HEIGHT - 50;

    for (uint i = 0; i < 300; ++i) {
        gas.spawn_random<NyaMolec>();
    }


    auto frame_start_time = chrono::system_clock::now();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return 0;
            }
        }

        window.clear();

        sf::RenderTexture back;
        back.create(WINDOW_WIDTH, WINDOW_HEIGHT);
        back.clear(sf::Color::Black);
        window.draw(sf::Sprite(back.getTexture()));

        gas.tick();

        sf::RenderTexture gas_texture;
        gas_texture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
        render(gas_texture, gas);
        window.draw(sf::Sprite(gas_texture.getTexture()));

        // display
        window.display();

        std::this_thread::sleep_until(frame_start_time + std::chrono::milliseconds(33));
    }
}