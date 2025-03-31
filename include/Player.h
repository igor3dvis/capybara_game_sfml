#pragma once

#include <SFML/Graphics.hpp>
#include "Constants.h"

class Player {
public:
    // Состояния анимации игрока
    enum class AnimState {
        Idle,
        Running,
        Jumping,
        Falling
    };

    Player();

    // Загрузка текстуры
    bool loadTexture(const std::string& filename);

    // Обновление состояния игрока
    void update(float deltaTime);

    // Отрисовка игрока
    void draw(sf::RenderWindow& window, bool showHitbox = false) const;

    // Перемещение игрока
    void move(float deltaTime);

    // Обработка прыжка
    void jump();

    // Проверка, на земле ли игрок
    bool isOnGround() const;

    // Установка флага нахождения на земле
    void setOnGround(bool onGround);

    // Получение позиции
    sf::Vector2f getPosition() const;

    // Установка позиции
    void setPosition(float x, float y);

    // Получение скорости
    sf::Vector2f getVelocity() const;

    // Установка скорости
    void setVelocity(float x, float y);

    // Обновление хитбокса
    void updateHitbox();

    // Получение границ для коллизий
    sf::FloatRect getCollisionBounds() const;

    // Увеличение счета
    void addScore(int points);

    // Получение текущего счета
    int getScore() const;

private:
    // Обновление анимации
    void updateAnimation(float deltaTime);

    // Настройка текстурных прямоугольников для анимации
    void setupAnimationFrames();

private:
    sf::Sprite sprite;             // Спрайт игрока
    sf::Texture texture;           // Текстура для спрайта
    sf::RectangleShape hitbox;     // Хитбокс для отображения

    float speedX = 0.0f;           // Горизонтальная скорость
    float speedY = 0.0f;           // Вертикальная скорость
    bool onGround = false;         // Находится ли игрок на земле
    float width = 68.0f;           // Ширина для коллизий (размер одного кадра)
    float height = 68.0f;          // Высота для коллизий
    bool facingRight = true;       // Направление персонажа (вправо/влево)
    int score = 0;                 // Счёт игрока

    // Переменные для анимации
    AnimState currentState = AnimState::Idle;  // Текущее состояние анимации
    std::vector<sf::IntRect> animationFrames;  // Кадры анимации
    float animationTime = 0.0f;                // Счетчик времени анимации
    float frameTime = 0.1f;                    // Время на один кадр анимации
    int currentFrame = 0;                      // Текущий индекс кадра
    bool animationDirection = true;            // true = вперед, false = назад (для пинг-понга)
    int framesInRow = 4;                       // Количество кадров в спрайт-листе
    int frameWidth = 68;                       // Ширина одного кадра
    int frameHeight = 68;                      // Высота одного кадра
    int jumpFrame = 3;                         // Индекс кадра для прыжка
    bool useWalkingAnim = true;                // Использовать ли анимацию ходьбы (пинг-понг)
    int walkStartFrame = 0;                    // Начальный кадр анимации ходьбы
    int walkEndFrame = 2;                      // Конечный кадр анимации ходьбы

};