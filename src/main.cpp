#include "common.h"
#include <SFML/Graphics.hpp>
#include "render.h"
#include <chrono>
#include <thread>

namespace chrono = std::chrono;

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);

Gas gas(Interval(0, 100), Interval(0, 100));
    gas.add(new NyaMolec(Point(30, 30, 0), Vector(0.1, 0.1, 0), 20));

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
        gas.tick();
        render(window, gas);

        // display
        window.display();

        std::this_thread::sleep_until(frame_start_time + std::chrono::milliseconds(33));
    }
}