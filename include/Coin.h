#pragma once

#include <SFML/Graphics.hpp>
#include "Constants.h"

class Coin {
public:
    Coin();
    Coin(float x, float y);

    // Добавим статический метод загрузки текстуры
    static bool loadTexture(const std::string& filename);
    
    // Метод для проверки загружена ли текстура
    static bool isTextureLoaded();
    
    // Инициализируем с указанием, использовать ли текстуру
    void init(float x, float y, bool useTextureAnimation = false);

    // Обновление состояния монеты
    void update(float time);

    // Отрисовка монеты
    void draw(sf::RenderWindow& window, bool showHitbox = false);

    // Обновление хитбокса
    void updateHitbox();

    // Получение границ монеты
    sf::FloatRect getBounds() const;

    // Установка состояния "собрана"
    void collect();

    // Проверка, собрана ли монета
    bool isCollected() const;

    // Получение стоимости монеты
    int getValue() const;

private:
    sf::CircleShape shape;
    sf::CircleShape hitbox;
    sf::Sprite sprite;               // Спрайт для текстурированной анимации
    static sf::Texture texture;      // Статическая текстура для всех монет
    static bool textureLoaded;       // Флаг загрузки текстуры

    bool collected = false;
    int value = GameConstants::COIN_VALUE;
    float initialY = 0.0f;           // Исходная Y-координата для анимации

    // Переменные для анимации спрайта
    std::vector<sf::IntRect> frames; // Кадры анимации
    float animationTime = 0.0f;      // Счетчик времени анимации
    float frameTime = 0.1f;          // Время на один кадр анимации
    int currentFrame = 0;            // Текущий индекс кадра
    int frameWidth = 32;             // Ширина одного кадра
    int frameHeight = 32;            // Высота одного кадра
    int framesCount = 3;             // Количество кадров в спрайт-листе
    bool useTexture = false;         // Использовать ли текстуру вместо фигуры

};