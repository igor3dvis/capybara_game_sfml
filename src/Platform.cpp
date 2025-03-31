#include "Platform.h"

Platform::Platform() {
    // Инициализация хитбокса
    hitbox.setFillColor(GameConstants::PLATFORM_HITBOX_COLOR);
}

Platform::Platform(float x, float y, float width, float height, const sf::Color& color) {
    init(x, y, width, height, color);
}

void Platform::init(float x, float y, float width, float height, const sf::Color& color) {
    shape.setSize(sf::Vector2f(width, height));
    shape.setPosition(x, y);
    shape.setFillColor(color);
    updateHitbox();
}

// Реализация метода draw с квалификатором const
void Platform::draw(sf::RenderWindow& window, bool showHitbox) const {
    window.draw(shape);

    if (showHitbox) {
        window.draw(hitbox);
    }
}

void Platform::updateHitbox() {
    sf::FloatRect bounds = shape.getGlobalBounds();
    hitbox.setSize(sf::Vector2f(bounds.width, bounds.height));
    hitbox.setPosition(bounds.left, bounds.top);
}

sf::FloatRect Platform::getBounds() const {
    return shape.getGlobalBounds();
}