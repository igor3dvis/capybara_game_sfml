#include "Button.h"

Button::Button() : currentState(State::Normal) {
    // Стандартные цвета
    normalColor = sf::Color(100, 100, 100);
    hoverColor = sf::Color(150, 150, 150);
    pressedColor = sf::Color(50, 50, 50);

    textNormalColor = sf::Color::White;
    textHoverColor = sf::Color::White;
    textPressedColor = sf::Color::White;

    shape.setFillColor(normalColor);
    shape.setOutlineThickness(2);
    shape.setOutlineColor(sf::Color::Black);

    text.setFillColor(textNormalColor);
}

Button::Button(const sf::Vector2f& position, const sf::Vector2f& size, const std::wstring& text,
    const sf::Font& font, unsigned int fontSize) : Button() {
    shape.setPosition(position);
    shape.setSize(size);

    this->text.setFont(font);
    this->text.setString(text);
    this->text.setCharacterSize(fontSize);

    // Центрирование текста в кнопке
    sf::FloatRect textBounds = this->text.getLocalBounds();
    this->text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    this->text.setPosition(position.x + size.x / 2.0f, position.y + size.y / 2.0f);
}

void Button::setPosition(const sf::Vector2f& position) {
    shape.setPosition(position);

    // Обновляем позицию текста
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    text.setPosition(position.x + shape.getSize().x / 2.0f, position.y + shape.getSize().y / 2.0f);
}

void Button::setSize(const sf::Vector2f& size) {
    shape.setSize(size);

    // Обновляем позицию текста
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    text.setPosition(shape.getPosition().x + size.x / 2.0f, shape.getPosition().y + size.y / 2.0f);
}

void Button::setText(const std::wstring& text) {
    this->text.setString(text);

    // Обновляем позицию текста
    sf::FloatRect textBounds = this->text.getLocalBounds();
    this->text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    this->text.setPosition(shape.getPosition().x + shape.getSize().x / 2.0f,
        shape.getPosition().y + shape.getSize().y / 2.0f);
}

void Button::setFont(const sf::Font& font) {
    text.setFont(font);
}

void Button::setTextSize(unsigned int size) {
    text.setCharacterSize(size);

    // Обновляем позицию текста
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.left + textBounds.width / 2.0f, textBounds.top + textBounds.height / 2.0f);
    text.setPosition(shape.getPosition().x + shape.getSize().x / 2.0f,
        shape.getPosition().y + shape.getSize().y / 2.0f);
}

void Button::setColors(const sf::Color& normal, const sf::Color& hover, const sf::Color& pressed) {
    normalColor = normal;
    hoverColor = hover;
    pressedColor = pressed;
    updateAppearance();
}

void Button::setTextColors(const sf::Color& normal, const sf::Color& hover, const sf::Color& pressed) {
    textNormalColor = normal;
    textHoverColor = hover;
    textPressedColor = pressed;
    updateAppearance();
}

void Button::setCallback(std::function<void()> callback) {
    this->callback = callback;
}

void Button::update(const sf::Vector2f& mousePosition) {
    sf::FloatRect bounds = shape.getGlobalBounds();

    if (bounds.contains(mousePosition)) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            currentState = State::Pressed;
        }
        else {
            currentState = State::Hover;
        }
    }
    else {
        currentState = State::Normal;
    }

    updateAppearance();
}

bool Button::handleEvent(const sf::Event& event, const sf::Vector2f& mousePosition) {
    sf::FloatRect bounds = shape.getGlobalBounds();

    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        if (bounds.contains(mousePosition)) {
            if (callback) {
                callback();
            }
            return true;
        }
    }

    return false;
}

void Button::draw(sf::RenderWindow& window) const {
    window.draw(shape);
    window.draw(text);
}

sf::FloatRect Button::getBounds() const {
    return shape.getGlobalBounds();
}

void Button::updateAppearance() {
    switch (currentState) {
    case State::Normal:
        shape.setFillColor(normalColor);
        text.setFillColor(textNormalColor);
        break;
    case State::Hover:
        shape.setFillColor(hoverColor);
        text.setFillColor(textHoverColor);
        break;
    case State::Pressed:
        shape.setFillColor(pressedColor);
        text.setFillColor(textPressedColor);
        break;
    }
}