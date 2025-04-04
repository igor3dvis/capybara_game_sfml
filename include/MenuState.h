#pragma once

#include "GameState.h"
#include "Button.h"
#include <vector>

class MenuState : public GameState {
public:
    MenuState(Game* game);
    virtual ~MenuState() {}

    // Реализация абстрактных методов GameState
    virtual void init() override;
    virtual void handleInput(sf::Event& event) override;
    virtual void update(float deltaTime) override;
    virtual void render(sf::RenderWindow& window) override;

private:
    // Фон меню
    sf::Sprite background;
    sf::Texture backgroundTexture;

    // Заголовок
    sf::Text titleText;

    // Кнопки меню
    std::vector<Button> buttons;

    // Шрифт для текста
    sf::Font font;

    // Настройка кнопок
    void setupButtons();

    // Обработчики нажатий на кнопки
    void onPlayButton();
    void onLevelSelectButton();
    void onSettingsButton();
    void onExitButton();
};