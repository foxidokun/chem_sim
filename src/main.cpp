#include "common.h"
#include <SFML/Graphics.hpp>
#include "render.h"
#include "gas.h"
#include "plot.h"
#include "button.h"
#include <chrono>
#include <thread>
#include <iostream>

static void add_buttons(ButtonManager& button_mgr, Gas& gas);
static void render_gas(sf::RenderWindow& window, const Gas& gas);
static void render_buttons(sf::RenderWindow& window, const ButtonManager& buttonmgr);
static void render_plot(sf::RenderWindow& window, const Plot& plot, const Point& pos, const Vector& size);

namespace chrono = std::chrono;


const Point  PRESS_PLOT_POS   (542,  40);
const Vector PRESS_PLOT_SIZE  (501, 170);
const Point  TEMP_PLOT_POS  (542, 223);
const Vector TEMP_PLOT_SIZE (501, 156);
const Point  COUNT1_PLOT_POS (542, 388);
const Vector COUNT1_PLOT_SIZE(230, 161);
const Point  COUNT2_PLOT_POS (790, 388);
const Vector COUNT2_PLOT_SIZE(262, 161);


int main() {
    sf::Texture background_texture;
    background_texture.loadFromFile(BACKGROUNG_PATH);
    sf::Sprite background(background_texture);

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);

    Gas gas(Interval(0, GAS_WIDTH), Interval(0, GAS_HEIGHT));
    gas.set_piston(GAS_HEIGHT - 50);

    Plot temp_plot(100);
    Plot pressure_plot(100);
    Plot count1_plot(100);
    Plot count2_plot(100);

    for (uint i = 0; i < 300; ++i) {
        gas.spawn_random<NyaMolec>();
    }

    ButtonManager buttonmgr;
    add_buttons(buttonmgr, gas);

    auto frame_start_time = chrono::system_clock::now();

    uint64_t frame_counter = 0;

    while (window.isOpen()) {
        window.clear();

        gas.tick();
        ++frame_counter;
        if (frame_counter > 10) {
            temp_plot.add_point(gas.temp());
            pressure_plot.add_point(gas.pressure());
            count1_plot.add_point(gas.counters()[0]);
            count2_plot.add_point(gas.counters()[1]);

            frame_counter = 0;
        }

        window.draw(background);
        render_gas(window, gas);
        render_buttons(window, buttonmgr);
        render_plot(window, temp_plot, TEMP_PLOT_POS, TEMP_PLOT_SIZE);
        render_plot(window, pressure_plot, PRESS_PLOT_POS, PRESS_PLOT_SIZE);
        render_plot(window, count1_plot, COUNT1_PLOT_POS, COUNT1_PLOT_SIZE);
        render_plot(window, count2_plot, COUNT2_PLOT_POS, COUNT2_PLOT_SIZE);
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
    gas->change_temp(1.1);
}

void decrease_temp_callback(const sf::Event&, void *args) {
    Gas *gas = static_cast<Gas *>(args);
    gas->change_temp(0.9);
}

void move_up_callback(const sf::Event&, void *args) {
    Gas *gas = static_cast<Gas *>(args);
    gas->set_piston(gas->piston() + PISTON_HEIGHT);
}

void move_down_callback(const sf::Event&, void *args) {
    Gas *gas = static_cast<Gas *>(args);
    gas->set_piston(gas->piston() - PISTON_HEIGHT);
}

static void add_buttons(ButtonManager& button_mgr, Gas& gas) {
    auto spawn_nya   = new TextureButton(Point(606, 41), 50, 50, spawn_btn_callback<NyaMolec>, &gas);
    auto spawn_meow  = new TextureButton(Point(673, 41), 50, 50, spawn_btn_callback<MeowMolec>, &gas);
    auto inc_temp    = new TextureButton(Point(742, 41), 50, 50, increase_temp_callback, &gas);
    auto dec_temp    = new TextureButton(Point(809, 41), 50, 50, decrease_temp_callback, &gas);
    auto up_piston   = new TextureButton(Point(873, 41), 50, 50, move_up_callback, &gas);
    auto down_piston = new TextureButton(Point(939, 41), 50, 50, move_down_callback, &gas);

    button_mgr.register_button(spawn_nya);
    button_mgr.register_button(spawn_meow);
    button_mgr.register_button(inc_temp);
    button_mgr.register_button(dec_temp);
    button_mgr.register_button(up_piston);
    button_mgr.register_button(down_piston);
}

static void render_gas(sf::RenderWindow& window, const Gas& gas) {
    sf::RenderTexture gas_texture;
    gas_texture.create(GAS_WIDTH, GAS_HEIGHT);
    sf::Sprite gas_block(gas_texture.getTexture());
    gas_block.setPosition(GAS_POS[0], GAS_POS[1]);

    gas.render(gas_texture);

    window.draw(gas_block);
}

static void render_buttons(sf::RenderWindow& window, const ButtonManager& buttonmgr) {
    sf::RenderTexture background;
    background.create(WINDOW_WIDTH, WINDOW_HEIGHT);

    buttonmgr.render(background);
    window.draw(sf::Sprite(background.getTexture()));
}

static void render_plot(sf::RenderWindow& window, const Plot& plot, const Point& pos, const Vector& size) {
    sf::RenderTexture plot_texture;
    plot_texture.create(size.x, size.y);
    plot_texture.clear();
    sf::Sprite plot_block(plot_texture.getTexture());
    plot_block.setPosition(pos.x, pos.y);

    plot.render(plot_texture);
    window.draw(plot_block);
}