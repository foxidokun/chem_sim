#include "common.h"
#include <SFML/Graphics.hpp>
#include "render.h"
#include "gas.h"
#include "plot.h"
#include "button.h"
#include <chrono>
#include <thread>
#include <iostream>

void add_buttons(ButtonManager& button_mgr, Gas& gas);
namespace chrono = std::chrono;

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);

    Gas gas(Interval(0, GAS_WIDTH), Interval(0, GAS_HEIGHT));
    Plot temp_plot(300);
    gas.piston_y = GAS_HEIGHT - 50;

    for (uint i = 0; i < 300; ++i) {
        gas.spawn_random<NyaMolec>();
    }

    ButtonManager buttonmgr;
    add_buttons(buttonmgr, gas);

    auto frame_start_time = chrono::system_clock::now();

    uint64_t frame_counter = 0;

    while (window.isOpen()) {
        window.clear();

        sf::RenderTexture back;
        back.create(WINDOW_WIDTH, WINDOW_HEIGHT);
        back.clear(sf::Color::Black);

        sf::RenderTexture gas_texture;
        gas_texture.create(GAS_WIDTH, GAS_HEIGHT);
        sf::Sprite gas_block(gas_texture.getTexture());
        gas_block.setPosition(20, 20);

        sf::RenderTexture plot_texture;
        plot_texture.create(600, 50);
        plot_texture.clear(sf::Color::Black);
        sf::Sprite plot_block(plot_texture.getTexture());
        plot_block.setPosition(400, 400);

        gas.tick();
        ++frame_counter;
        if (frame_counter > 5) {
            temp_plot.add_point(gas.temp());
            frame_counter = 0;
        }

        gas.render(gas_texture);
        temp_plot.render(plot_texture);
        buttonmgr.render(back);

        window.draw(sf::Sprite(back.getTexture()));
        window.draw(gas_block);
        window.draw(plot_block);
        window.display();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return 0;
            } else if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased) {
                buttonmgr.dispatch(window, event);
            }
        }

        std::this_thread::sleep_until(frame_start_time + std::chrono::milliseconds(33));
    }
}

template<typename T>
void spawn_btn_callback(const sf::Event&, void *args) {
    Gas *gas = static_cast<Gas *>(args);

    for (uint i = 0; i < 10; ++i) {
        gas->spawn_random<T>();
    }
}

void increase_temp_callback(const sf::Event&, void *args) {
    Gas *gas = static_cast<Gas *>(args);
    gas->change_temp(0.01);
}

void decrease_temp_callback(const sf::Event&, void *args) {
    Gas *gas = static_cast<Gas *>(args);
    gas->change_temp(-0.01);
}

void add_buttons(ButtonManager& button_mgr, Gas& gas) {
    auto spawn_nya  = new TextureButton(Point(600, 0), 40, 30, spawn_btn_callback<NyaMolec>, &gas);
    auto spawn_meow = new TextureButton(Point(650, 0), 40, 30, spawn_btn_callback<MeowMolec>, &gas);
    auto inc_temp   = new TextureButton(Point(700, 0), 40, 30, increase_temp_callback, &gas);
    auto dec_temp   = new TextureButton(Point(750, 0), 40, 30, decrease_temp_callback, &gas);

    button_mgr.register_button(spawn_nya);
    button_mgr.register_button(spawn_meow);
    button_mgr.register_button(inc_temp);
    button_mgr.register_button(dec_temp);
}