#include "../include/game.hpp"

void Game::initVariables() {
    this->window = nullptr;

    this->points = 0;
    this->enemySpawnTimerMax = 1000.f;
    this->enemySpawnTimer = this->enemySpawnTimerMax;
    this->maxEnemies = 5;
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


void Game::spawnEnemy() {
    this->enemy.setPosition(
        static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
        0.f
    );

    this->enemy.setFillColor(sf::Color::Green);
    this->enemies.push_back(this->enemy);
}


void Game::pollEvents() {
  while (this->window->pollEvent(this->ev)) {
      
        if (this->ev.type == sf::Event::Closed) this->window->close();
        if (this->ev.key.code == sf::Keyboard::Escape) this->window->close();
    }
}


void Game::updateMousePosition() {
    this->mousePosWindow = sf::Mouse::getPosition(*this->window);
}


void Game::updateEnemies() {
    if (this->enemies.size() < this->maxEnemies) {
        if (this->enemySpawnTimer >= this->enemySpawnTimerMax) {
            this->spawnEnemy();
            this->enemySpawnTimer = 0.f;
        } else {
            this->enemySpawnTimer += 1.f;
        }
    }

    for (auto &e : this->enemies) {
        e.move(0.f, 1.f);
    }
}


void Game::update() {
    this->pollEvents();
    this->updateMousePosition();
    this->updateEnemies();
}


void Game::renderEnemies() {
     for (auto &e : this->enemies) {
        this->window->draw(e);
    }
}


void Game::render() {
    this->window->clear();
    this->renderEnemies();
    this->window->display();
}

