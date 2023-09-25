#include "button.h"

void TextureButton::render(sf::RenderTexture& window) const {
    window.draw(_sprite);
}

void ButtonManager::dispatch(const sf::RenderTarget& window, const sf::Event& event) {
    assert (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased);

    for (uint i = 0; i < buttons.size(); ++i) {
        if (buttons[i]->is_hit(window, event)) { // non virtual check
            buttons[i]->react(event);    // virtual call
        }
    }
}

void ButtonManager::render(sf::RenderTexture& window) const {
    for (uint i = 0; i < buttons.size(); ++i) {
        buttons[i]->render(window);
    }
}
