#include "Coin.h"
#include <cmath>
#include <iostream>


// Инициализация статических полей
sf::Texture Coin::texture;
bool Coin::textureLoaded = false;

// Статический метод загрузки текстуры
bool Coin::loadTexture(const std::string& filename) {
    if (!textureLoaded) {
        if (!texture.loadFromFile(filename)) {
            std::cout << "Не удалось загрузить текстуру для монет: " << filename << std::endl;
            return false;
        }
        textureLoaded = true;
    }
    return true;
}

// Метод для проверки, загружена ли текстура
bool Coin::isTextureLoaded() {
    return textureLoaded;
}

Coin::Coin() {
    // Инициализация хитбокса
    hitbox.setFillColor(GameConstants::COIN_HITBOX_COLOR);
}

Coin::Coin(float x, float y) {
    init(x, y);
}

void Coin::init(float x, float y, bool useTextureAnimation) {
    // Сохраняем выбор использования текстуры
    useTexture = useTextureAnimation;

    // Инициализация формы для отрисовки без текстуры (для обратной совместимости)
    shape.setRadius(GameConstants::COIN_RADIUS);
    shape.setFillColor(GameConstants::COIN_COLOR);
    shape.setPosition(x, y);
    shape.setOrigin(GameConstants::COIN_RADIUS, GameConstants::COIN_RADIUS);

    // Инициализация спрайта если используем текстуру
    if (useTexture && textureLoaded) {
        // Создаем кадры анимации
        frames.clear();

        // Проверяем размер текстуры
        sf::Vector2u textureSize = texture.getSize();
        int maxPossibleFrames = textureSize.x / frameWidth;
        int actualFrames = std::min(framesCount, maxPossibleFrames);

        if (actualFrames < framesCount) {
            std::cout << "Предупреждение: текстура монет слишком маленькая для " << framesCount
                << " кадров. Используем только " << actualFrames << " кадров." << std::endl;
            // Обновляем framesCount
            framesCount = actualFrames;
        }

        for (int i = 0; i < framesCount; ++i) {
            frames.push_back(sf::IntRect(i * frameWidth, 0, frameWidth, frameHeight));
        }

        // Настраиваем спрайт
        sprite.setTexture(texture);
        if (!frames.empty()) {
            sprite.setTextureRect(frames[0]);
        }
        sprite.setOrigin(frameWidth / 2, frameHeight / 2);
        sprite.setPosition(x, y);
    }

    initialY = y;
    updateHitbox();
}


void Coin::update(float time) {
    if (!collected) {
        float yPos = initialY + std::sin(time * 3.0f) * 0.5f;

        if (useTexture && textureLoaded) {
            // Обновляем позицию спрайта
            sprite.setPosition(sprite.getPosition().x, yPos);

            // Обновляем анимацию спрайта
            animationTime += 0.016f; // Примерно 60 FPS
            if (animationTime >= frameTime) {
                animationTime = 0;
                // ПРОВЕРКА: безопасно обновляем индекс кадра
                if (!frames.empty()) {
                    currentFrame = (currentFrame + 1) % frames.size();
                    sprite.setTextureRect(frames[currentFrame]);
                }
            }
        }
        else {
            // Обновляем позицию фигуры (старый вариант)
            shape.setPosition(shape.getPosition().x, yPos);
        }

        updateHitbox();
    }
}

void Coin::draw(sf::RenderWindow& window, bool showHitbox) {
    if (!collected) {
        if (useTexture) {
            window.draw(sprite);
        }
        else {
            window.draw(shape);
        }

        if (showHitbox) {
            window.draw(hitbox);
        }
    }
}

void Coin::updateHitbox() {
    sf::FloatRect bounds;

    if (useTexture) {
        bounds = sprite.getGlobalBounds();
    }
    else {
        bounds = shape.getGlobalBounds();
    }

    float diameter = bounds.width;
    hitbox.setRadius(diameter / 2);
    hitbox.setOrigin(diameter / 2, diameter / 2);

    if (useTexture) {
        hitbox.setPosition(sprite.getPosition());
    }
    else {
        hitbox.setPosition(shape.getPosition());
    }
}

sf::FloatRect Coin::getBounds() const {
    if (useTexture) {
        return sprite.getGlobalBounds();
    }
    else {
        return shape.getGlobalBounds();
    }
}

void Coin::collect() {
    collected = true;
}

bool Coin::isCollected() const {
    return collected;
}

int Coin::getValue() const {
    return value;
}