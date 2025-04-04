#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

class Button {
public:
    Button();
    
    Button(const sf::Vector2f& position, const sf::Vector2f& size, const std::wstring& text,
        const sf::Font& font, unsigned int fontSize = 24);

    // Остальные методы...

    


    // Настройка кнопки
    void setPosition(const sf::Vector2f& position);
    void setSize(const sf::Vector2f& size);
    void setText(const std::wstring& text);
    void setFont(const sf::Font& font);
    void setTextSize(unsigned int size);
    void setColors(const sf::Color& normal, const sf::Color& hover, const sf::Color& pressed);
    void setTextColors(const sf::Color& normal, const sf::Color& hover, const sf::Color& pressed);

    // Установка действия, которое будет выполнено при нажатии на кнопку
    void setCallback(std::function<void()> callback);

    // Обновление состояния кнопки (для проверки наведения мыши)
    void update(const sf::Vector2f& mousePosition);

    // Обработка события нажатия мыши
    bool handleEvent(const sf::Event& event, const sf::Vector2f& mousePosition);

    // Отрисовка кнопки
    void draw(sf::RenderWindow& window) const;

    // Получение размеров и позиции
    sf::FloatRect getBounds() const;

private:
    sf::RectangleShape shape;
    sf::Text text;

    // Цвета для разных состояний
    sf::Color normalColor;
    sf::Color hoverColor;
    sf::Color pressedColor;

    sf::Color textNormalColor;
    sf::Color textHoverColor;
    sf::Color textPressedColor;

    // Состояние кнопки
    enum class State { Normal, Hover, Pressed };
    State currentState;

    // Функция обратного вызова
    std::function<void()> callback;

    // Обновление внешнего вида в зависимости от состояния
    void updateAppearance();
};