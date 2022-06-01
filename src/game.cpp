#include "../include/game.hpp"

void Game::initVariables() {
    this->window = nullptr;
}

void Game::initWindow() {
    this->videoMode.width = 800;
    this->videoMode.height = 600;
    this->window = new sf::RenderWindow(this->videoMode, "Game Window");
    this->window->setFramerateLimit(60);
}


void Game::initEnemies() {
    this->enemy.setPosition(10.f, 10.f);
    this->enemy.setSize(sf::Vector2f(100.f, 100.f));
    this->enemy.setScale(0.5f, 0.5f);
    this->enemy.setFillColor(sf::Color::Red);
    this->enemy.setOutlineColor(sf::Color::Cyan);
    this->enemy.setOutlineThickness(1.f);
}


Game::Game() {
    this->initVariables();
    this->initWindow();
    this->initEnemies();
}

Game::~Game() {
    delete this->window;
}


const bool Game::isRunning() const {
    return this->window->isOpen();
}


void Game::pollEvents() {
  while (this->window->pollEvent(this->ev)) {
      
        if (this->ev.type == sf::Event::Closed) this->window->close();
        if (this->ev.key.code == sf::Keyboard::Escape) this->window->close();
    }
}


void Game::update() {
    this->pollEvents();
}


void Game::render() {
    this->window->clear();
    this->window->draw(this->enemy);
    this->window->display();
}

