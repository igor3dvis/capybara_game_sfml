#include "MenuState.h"
#include "Game.h"
#include "GameplayState.h"
#include "Constants.h"
#include <iostream>

MenuState::MenuState(Game* game) : GameState(game) {
}

void MenuState::init() {
    // Загрузка шрифта
    if (!font.loadFromFile("assets/fonts/arial.ttf") &&
        !font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf") &&
        !font.loadFromFile("fonts\\arial.ttf")) {
        std::cout << "Can not upload font for Menu!" << std::endl;
    }

    // Настройка заголовка
    titleText.setFont(font);
    titleText.setString(L"Капибара-Платформер");
    titleText.setCharacterSize(50);
    titleText.setFillColor(sf::Color::White);
    titleText.setOutlineColor(sf::Color::Black);
    titleText.setOutlineThickness(3);

    // Центрируем заголовок
    sf::FloatRect titleBounds = titleText.getLocalBounds();
    titleText.setOrigin(titleBounds.left + titleBounds.width / 2.0f, titleBounds.top + titleBounds.height / 2.0f);
    titleText.setPosition(GameConstants::WINDOW_WIDTH / 2.0f, 100.0f);

    // Попытка загрузить фон
    if (!backgroundTexture.loadFromFile("assets/textures/menu_background.png")) {
        std::cout << "Texture for Background did't upload. Using colore background" << std::endl;
        // Если текстура не загружена, фон будет отрисован как прямоугольник в render()
    }
    else {
        background.setTexture(backgroundTexture);
        // Масштабируем фон под размер окна
        float scaleX = static_cast<float>(GameConstants::WINDOW_WIDTH) / backgroundTexture.getSize().x;
        float scaleY = static_cast<float>(GameConstants::WINDOW_HEIGHT) / backgroundTexture.getSize().y;
        background.setScale(scaleX, scaleY);
    }

    // Настройка кнопок
    setupButtons();
}

void MenuState::setupButtons() {
    // Размеры и позиции для кнопок
    const float buttonWidth = 250.0f;
    const float buttonHeight = 60.0f;
    const float buttonY = 250.0f;
    const float buttonYSpacing = 80.0f;
    const float buttonX = (GameConstants::WINDOW_WIDTH - buttonWidth) / 2.0f;

    // Создаем кнопки
    buttons.clear();

    // Кнопка "Играть"
    Button playButton(sf::Vector2f(buttonX, buttonY),
        sf::Vector2f(buttonWidth, buttonHeight),
        L"Играть", font, 28);
    playButton.setColors(sf::Color(100, 150, 100), sf::Color(120, 180, 120), sf::Color(80, 120, 80));
    playButton.setCallback([this]() { onPlayButton(); });
    buttons.push_back(playButton);

    // Кнопка "Выбор уровня"
    Button levelSelectButton(sf::Vector2f(buttonX, buttonY + buttonYSpacing),
        sf::Vector2f(buttonWidth, buttonHeight),
        L"Выбор уровня", font, 28);
    levelSelectButton.setColors(sf::Color(100, 100, 150), sf::Color(120, 120, 180), sf::Color(80, 80, 120));
    levelSelectButton.setCallback([this]() { onLevelSelectButton(); });
    buttons.push_back(levelSelectButton);

    // Кнопка "Настройки"
    Button settingsButton(sf::Vector2f(buttonX, buttonY + 2 * buttonYSpacing),
        sf::Vector2f(buttonWidth, buttonHeight),
        L"Настройки", font, 28);
    settingsButton.setColors(sf::Color(150, 150, 100), sf::Color(180, 180, 120), sf::Color(120, 120, 80));
    settingsButton.setCallback([this]() { onSettingsButton(); });
    buttons.push_back(settingsButton);

    // Кнопка "Выход"
    Button exitButton(sf::Vector2f(buttonX, buttonY + 3 * buttonYSpacing),
        sf::Vector2f(buttonWidth, buttonHeight),
        L"Выход", font, 28);
    exitButton.setColors(sf::Color(150, 100, 100), sf::Color(180, 120, 120), sf::Color(120, 80, 80));
    exitButton.setCallback([this]() { onExitButton(); });
    buttons.push_back(exitButton);
}

void MenuState::handleInput(sf::Event& event) {
    sf::Vector2f mousePos;

    if (event.type == sf::Event::MouseMoved) {
        mousePos = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);

        // Обновляем состояние кнопок при движении мыши
        for (auto& button : buttons) {
            button.update(mousePos);
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased) {
        mousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);

        // Проверяем нажатие на кнопки
        for (auto& button : buttons) {
            if (button.handleEvent(event, mousePos)) {
                // Если кнопка обработала событие, прекращаем проверку других кнопок
                break;
            }
        }
    }
    else if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            // Escape закрывает игру из меню
            game->quit();
        }
    }
}

void MenuState::update(float deltaTime) {
    // В меню нам нужно только обрабатывать ввод, 
    // но здесь можно добавить анимации или эффекты
}

void MenuState::render(sf::RenderWindow& window) {
    // Очищаем окно цветом фона
    window.clear(sf::Color(70, 130, 180)); // Небесно-голубой фон, если текстура не загружена

    // Рисуем фон, если текстура загружена
    if (backgroundTexture.getSize().x > 0) {
        window.draw(background);
    }

    // Рисуем заголовок
    window.draw(titleText);

    // Рисуем кнопки
    for (auto& button : buttons) {
        button.draw(window);
    }
}

// Обработчики нажатий на кнопки
void MenuState::onPlayButton() {
    // Переход в игровой режим
    game->changeState(std::make_unique<GameplayState>(game, 1)); // Начинаем с уровня 1
}

void MenuState::onLevelSelectButton() {
    // TODO: Реализовать экран выбора уровня
    std::cout << "The chois of levels is not realised yet! (in the process)" << std::endl;
}

void MenuState::onSettingsButton() {
    // TODO: Реализовать экран настроек
    std::cout << "The Settings is not realized yet (in the process)" << std::endl;
}

void MenuState::onExitButton() {
    // Выход из игры
    game->quit();
}