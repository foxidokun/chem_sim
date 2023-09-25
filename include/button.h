#include "common.h"
#include <SFML/Graphics.hpp>
#include "point.h"
#include "render.h"
#include "dynarray.h"
#include <assert.h>
#include <functional>

using onclick_func_t = void(*)(const sf::Event&, void*);

class Button: public Renderable {
    onclick_func_t _on_click;
    void * _callback_args;

    Point  _pos;
    double _width;
    double _height;

public:
    Button(Point pos, double width, double height, onclick_func_t callback, void *callback_args):
        _pos(pos),
        _width(width),
        _height(height),
        _on_click(callback),
        _callback_args(callback_args)
        {}

    bool is_hit(const sf::RenderTarget& window, const sf::Event& event) const noexcept {
        assert (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased);

        sf::Vector2f mouse_coords = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
        mouse_coords.y = WINDOW_HEIGHT - mouse_coords.y;

        return (_pos.x <= mouse_coords.x && mouse_coords.x <= _pos.x + _width) && 
               (_pos.y <= mouse_coords.y && mouse_coords.y <= _pos.y + _height);
    }

    // Default implementation
    virtual void react(const sf::Event& event) {
        _on_click(event, _callback_args);
    }

    virtual ~Button() = default;
};

class TextureButton: public Button {
    sf::Sprite _sprite;

public:
    TextureButton(Point pos, double width, double height, sf::Texture texture, onclick_func_t callback,
                                                                                                void *callback_args):
        Button(pos, width, height, callback, callback_args),
        _sprite(sf::Sprite(texture))
        {
            _sprite.setPosition(pos.x, pos.y);
        }

    TextureButton(Point pos, double width, double height, onclick_func_t callback, void *callback_args):
        Button(pos, width, height, callback, callback_args)
        {
            sf::Texture texture;
            texture.create(width, height);
            _sprite = sf::Sprite(texture);
            _sprite.setColor(sf::Color::Yellow);
            _sprite.setPosition(pos.x, pos.y);
        }

    void render(sf::RenderTexture& window) const final;
};

class ButtonManager: public Renderable {
    dynarray<Button *> buttons;

public:
    ButtonManager() = default;
    ButtonManager(const ButtonManager&) = delete;
    ButtonManager(ButtonManager&& other) noexcept { // for std::vector and others
        std::swap(buttons, other.buttons);
    }

    void register_button(Button *btn) {
        buttons.push_back(btn);
    }

    void dispatch(const sf::RenderTarget& window, const sf::Event& event);

    void render(sf::RenderTexture& window) const;
};