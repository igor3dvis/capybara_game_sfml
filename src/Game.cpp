#include "Game.h"
#include "GameState.h"
#include "MenuState.h"
#include <iostream>

Game::Game() : running(false) {
}

Game::~Game() {
    // Очищаем все состояния при уничтожении игры
    while (!states.empty()) {
        states.pop();
    }
}

bool Game::init() {
    // Создаем окно
    window.create(sf::VideoMode(GameConstants::WINDOW_WIDTH, GameConstants::WINDOW_HEIGHT),
        L"Платформер с капибарой");
    window.setFramerateLimit(GameConstants::FRAMERATE_LIMIT);

    // Устанавливаем начальное состояние - меню
    pushState(std::make_unique<MenuState>(this));

    running = true;
    return true;
}

void Game::run() {
    while (running && window.isOpen()) {
        // Измеряем прошедшее время
        float deltaTime = clock.restart().asSeconds() * GameConstants::GAME_SPEED;

        // Обрабатываем события
        processEvents();

        // Обновляем игровую логику
        update(deltaTime);

        // Отрисовываем игру
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // Передаем событие текущему состоянию
        if (!states.empty()) {
            states.top()->handleInput(event);
        }
    }
}

void Game::update(float deltaTime) {
    // Обновляем текущее состояние (если оно есть)
    if (!states.empty()) {
        states.top()->update(deltaTime);
    }
}

void Game::render() {
    // Очищаем окно
    window.clear();

    // Отрисовываем текущее состояние (если оно есть)
    if (!states.empty()) {
        states.top()->render(window);
    }

    // Отображаем содержимое окна
    window.display();
}

void Game::pushState(std::unique_ptr<GameState> state) {
    // Если есть текущее состояние, ставим его на паузу
    if (!states.empty()) {
        states.top()->pause();
    }

    // Инициализируем новое состояние
    state->init();

    // Добавляем состояние в стек
    states.push(std::move(state));
}

void Game::popState() {
    // Удаляем текущее состояние
    if (!states.empty()) {
        states.pop();
    }

    // Восстанавливаем предыдущее состояние, если оно есть
    if (!states.empty()) {
        states.top()->resume();
    }
    else {
        // Если больше нет состояний, завершаем работу
        quit();
    }
}

void Game::changeState(std::unique_ptr<GameState> state) {
    // Удаляем текущее состояние
    if (!states.empty()) {
        states.pop();
    }

    // Инициализируем новое состояние
    state->init();

    // Добавляем его в стек
    states.push(std::move(state));
}

GameState* Game::getCurrentState() {
    if (states.empty()) {
        return nullptr;
    }

    return states.top().get();
}

void Game::quit() {
    running = false;
}