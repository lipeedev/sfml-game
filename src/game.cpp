#include "../include/game.hpp"

void Game::initVariables() {
    this->window = nullptr;
}

void Game::initWindow() {
    this->videoMode.width = 800;
    this->videoMode.height = 600;

    this->window = new sf::RenderWindow(this->videoMode, "Game Window");
}


Game::Game() {
    this->initVariables();
    this->initWindow();
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
    
    this->window->display();
}

