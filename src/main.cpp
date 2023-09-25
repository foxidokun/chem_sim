#include "common.h"
#include <SFML/Graphics.hpp>
#include "render.h"
#include "gas.h"
#include "button.h"
#include <chrono>
#include <thread>
#include <iostream>

void add_buttons(ButtonManager& button_mgr, Gas& gas);
namespace chrono = std::chrono;

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);

    Gas gas(Interval(0, GAS_WIDTH), Interval(0, GAS_HEIGHT));
    gas.piston_y = GAS_HEIGHT - 50;

    for (uint i = 0; i < 300; ++i) {
        gas.spawn_random<NyaMolec>();
    }

    ButtonManager buttonmgr;
    add_buttons(buttonmgr, gas);

    auto frame_start_time = chrono::system_clock::now();

    while (window.isOpen()) {
        window.clear();

        sf::RenderTexture back;
        back.create(WINDOW_WIDTH, WINDOW_HEIGHT);
        back.clear(sf::Color::Black);

        sf::RenderTexture gas_texture;
        gas_texture.create(GAS_WIDTH, GAS_HEIGHT);
        sf::Sprite gas_block(gas_texture.getTexture());
        gas_block.setPosition(20, 20);

        gas.tick();
        gas.render(gas_texture);
        buttonmgr.render(back);

        window.draw(sf::Sprite(back.getTexture()));
        window.draw(gas_block);
        window.display();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return 0;
            } else if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased) {
                auto cc = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

                buttonmgr.dispatch(window, event);
            }
        }

        std::this_thread::sleep_until(frame_start_time + std::chrono::milliseconds(33));
    }
}

void callback_a(const sf::Event&) {
    std::cout << "A\n";
}

void callback_и(const sf::Event&) {
    std::cout << "B\n";
}
void callback_с(const sf::Event&) {
    std::cout << "C\n";
}
void callback_в(const sf::Event&) {
    std::cout << "D\n";
}

template<typename T>
void spawn_btn_callback(const sf::Event&, void *args) {
    Gas *gas = static_cast<Gas *>(args);

    for (uint i = 0; i < 10; ++i) {
        gas->spawn_random<T>();
    }
}

void add_buttons(ButtonManager& button_mgr, Gas& gas) {
    auto spawn_nya  = new TextureButton(Point(600, 0), 50, 30, spawn_btn_callback<NyaMolec>, &gas);
    auto spawn_meow = new TextureButton(Point(640, 0), 50, 30, spawn_btn_callback<MeowMolec>, &gas);

    button_mgr.register_button(spawn_nya);
    button_mgr.register_button(spawn_meow);
}