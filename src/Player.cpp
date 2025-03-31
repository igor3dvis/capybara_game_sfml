#include "Player.h"
#include <iostream>

Player::Player() {
    // Инициализация хитбокса
    hitbox.setFillColor(GameConstants::PLAYER_HITBOX_COLOR);
}

bool Player::loadTexture(const std::string& filename) {
    if (!texture.loadFromFile(filename)) {
        std::cout << "Не удалось загрузить текстуру " << filename << "!" << std::endl;
        std::cout << "Создаем пустую текстуру 272x68 пикселей." << std::endl;

        // Создаем пустую текстуру в случае ошибки
        texture.create(272, 68);
        sf::Image img;
        img.create(272, 68, sf::Color::Red);
        texture.update(img);
    }

    // Настраиваем спрайт
    sprite.setTexture(texture);

    // Настраиваем кадры анимации
    setupAnimationFrames();

    // Устанавливаем первый кадр
    sprite.setTextureRect(animationFrames[0]);

    // Устанавливаем центр спрайта (точку поворота)
    sprite.setOrigin(frameWidth / 2, 0);
    sprite.setScale(1, 1); // Начальный масштаб (вправо)

    return true;
}

void Player::setupAnimationFrames() {
    // Очищаем предыдущие кадры, если они есть
    animationFrames.clear();

    // Получаем размеры текстуры для проверки
    sf::Vector2u textureSize = texture.getSize();

    // Проверяем, достаточно ли широкая текстура для всех кадров
    int maxPossibleFrames = textureSize.x / frameWidth;
    int actualFrames = std::min(framesInRow, maxPossibleFrames);

    if (actualFrames < framesInRow) {
        std::cout << "Предупреждение: текстура слишком маленькая для " << framesInRow
            << " кадров. Используем только " << actualFrames << " кадров." << std::endl;
        // Обновляем framesInRow, чтобы другие методы знали о реальном количестве кадров
        framesInRow = actualFrames;
    }

    // Добавляем все доступные кадры из спрайт-листа
    for (int i = 0; i < framesInRow; ++i) {
        animationFrames.push_back(sf::IntRect(i * frameWidth, 0, frameWidth, frameHeight));
    }

    // Проверяем, что jumpFrame находится в допустимом диапазоне
    if (jumpFrame >= animationFrames.size()) {
        jumpFrame = animationFrames.size() - 1;
        std::cout << "Предупреждение: jumpFrame вне диапазона, установлен в " << jumpFrame << std::endl;
    }

    // Устанавливаем начальный кадр
    currentFrame = 0;
    if (!animationFrames.empty()) {
        sprite.setTextureRect(animationFrames[currentFrame]);
    }
}

void Player::update(float deltaTime) {
    // Обработка движения
    speedX = 0.0f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        speedX = -GameConstants::PLAYER_SPEED;

        // Поворачиваем спрайт влево (если еще не повернут)
        if (facingRight) {
            sprite.setScale(-1, 1); // Отражаем по горизонтали
            facingRight = false;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        speedX = GameConstants::PLAYER_SPEED;

        // Поворачиваем спрайт вправо (если еще не повернут)
        if (!facingRight) {
            sprite.setScale(1, 1); // Нормальный масштаб
            facingRight = true;
        }
    }

    // Определяем текущее состояние анимации
    if (!onGround) {
        // В воздухе
        if (speedY < 0) {
            currentState = AnimState::Jumping;
        }
        else {
            currentState = AnimState::Falling;
        }
    }
    else {
        // На земле
        if (speedX != 0) {
            currentState = AnimState::Running;
        }
        else {
            currentState = AnimState::Idle;
        }
    }

    // Применяем гравитацию с ограничением максимальной скорости падения
    speedY += GameConstants::GRAVITY * deltaTime;

    // Ограничиваем максимальную скорость падения
    if (speedY > GameConstants::MAX_FALL_SPEED) {
        speedY = GameConstants::MAX_FALL_SPEED;
    }

    // Обновляем анимацию
    updateAnimation(deltaTime);

    // Перемещаем игрока
    move(deltaTime);

    // Проверяем выход за границы экрана
    sf::Vector2f position = sprite.getPosition();

    if (position.x < width / 2) {
        sprite.setPosition(width / 2, position.y);
    }

    if (position.x > GameConstants::WINDOW_WIDTH - width / 2) {
        sprite.setPosition(GameConstants::WINDOW_WIDTH - width / 2, position.y);
    }

    // Обновляем хитбокс
    updateHitbox();
}

void Player::updateAnimation(float deltaTime) {
    // Переключаемся между типами анимации в зависимости от состояния
    if (currentState == AnimState::Jumping || currentState == AnimState::Falling) {
        // Для прыжка используем специальный кадр
        // ПРОВЕРКА: убедимся, что jumpFrame находится в пределах вектора
        if (jumpFrame >= 0 && jumpFrame < animationFrames.size()) {
            currentFrame = jumpFrame;
        }
        else {
            // Защитный код: если кадр прыжка недействителен, используем первый кадр
            currentFrame = 0;
            // Можно также вывести предупреждение для отладки
            std::cout << "Предупреждение: недопустимый кадр прыжка: " << jumpFrame << std::endl;
        }
        useWalkingAnim = false;
    }
    else {
        // Для других состояний используем анимацию ходьбы (пинг-понг)
        useWalkingAnim = true;
    }

    // Если игрок находится в состоянии, где нужна анимация ходьбы
    if (useWalkingAnim) {
        // Обновляем счетчик времени анимации
        animationTime += deltaTime;

        // Если прошло достаточно времени, меняем кадр
        if (animationTime >= frameTime) {
            // Сбрасываем счетчик
            animationTime = 0;

            // Обновляем индекс кадра в стиле пинг-понг (только между 0 и 2)
            if (animationDirection) {
                // Движение вперед
                currentFrame++;
                // ПРОВЕРКА: не выходим ли за пределы диапазона walkEndFrame
                if (currentFrame >= walkEndFrame || currentFrame >= animationFrames.size()) {
                    currentFrame = std::min(walkEndFrame, static_cast<int>(animationFrames.size()) - 1);
                    animationDirection = false; // Меняем направление на обратное
                }
            }
            else {
                // Движение назад
                currentFrame--;
                // ПРОВЕРКА: не выходим ли за пределы диапазона walkStartFrame
                if (currentFrame <= walkStartFrame || currentFrame < 0) {
                    currentFrame = std::max(walkStartFrame, 0);
                    animationDirection = true; // Меняем направление на прямое
                }
            }
        }
    }

    // ПРОВЕРКА: убедимся, что currentFrame находится в пределах вектора animationFrames
    if (currentFrame >= 0 && currentFrame < animationFrames.size()) {
        // Устанавливаем текущий кадр
        sprite.setTextureRect(animationFrames[currentFrame]);
    }
    else {
        // Если индекс недопустимый, используем первый кадр и выведем предупреждение
        currentFrame = 0;
        std::cout << "Предупреждение: недопустимый индекс кадра: " << currentFrame
            << " (размер: " << animationFrames.size() << ")" << std::endl;
        if (!animationFrames.empty()) {
            sprite.setTextureRect(animationFrames[0]);
        }
    }
}

void Player::draw(sf::RenderWindow& window, bool showHitbox) const {
    window.draw(sprite);

    if (showHitbox) {
        window.draw(hitbox);
    }
}

void Player::move(float deltaTime) {
    sprite.move(speedX * deltaTime, speedY * deltaTime);
}

void Player::jump() {
    if (onGround) {
        speedY = -GameConstants::JUMP_POWER;
        onGround = false;
    }
}

bool Player::isOnGround() const {
    return onGround;
}

void Player::setOnGround(bool ground) {
    onGround = ground;
}

sf::Vector2f Player::getPosition() const {
    return sprite.getPosition();
}

void Player::setPosition(float x, float y) {
    sprite.setPosition(x, y);
    updateHitbox();
}

sf::Vector2f Player::getVelocity() const {
    return { speedX, speedY };
}

void Player::setVelocity(float x, float y) {
    speedX = x;
    speedY = y;
}

void Player::updateHitbox() {
    hitbox.setSize(sf::Vector2f(width, height));
    hitbox.setOrigin(width / 2, 0); // Такой же origin как у спрайта
    hitbox.setPosition(sprite.getPosition());
}

sf::FloatRect Player::getCollisionBounds() const {
    // Создаем прямоугольник для коллизии игрока
    // Добавляем небольшие поля (-2 пикселя по ширине с каждой стороны)
    // для более точной коллизии
    return sf::FloatRect(
        sprite.getPosition().x - (width / 2) + 2,  // Левый край + поле
        sprite.getPosition().y,                    // Верхний край
        width - 4,                                 // Ширина минус поля
        height                                     // Высота
    );
}

void Player::addScore(int points) {
    score += points;
}

int Player::getScore() const {
    return score;
}