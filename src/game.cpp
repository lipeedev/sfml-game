#include "../include/game.hpp"

void Game::initVariables() {
    this->window = nullptr;

    this->endGame = false;
    this->points = 0;
    this->health = 10;
    this->enemySpawnTimerMax = 10.f;
    this->enemySpawnTimer = this->enemySpawnTimerMax;
    this->maxEnemies = 6;
    this->mouseHelds = false;
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


const bool Game::getEndGame() const { 
    return this->endGame;
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
    this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
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

    for (int i = 0; i < this->enemies.size(); i++) {
        bool deleted = false;

        this->enemies[i].move(0.f, 3.f);

        if (this->enemies[i].getPosition().y > this->window->getSize().y) {
            this->enemies.erase(this->enemies.begin() + i);
            if (this->health > 0) this->health -= 1;
        }
    }
     
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (!this->mouseHelds) {

            this->mouseHelds = true;
            bool deleted = false;

            for (int i = 0; i < this->enemies.size() && !deleted; i++) {
                if (this->enemies[i].getGlobalBounds().contains(this->mousePosView)) {
                    deleted = true;
                    this->enemies.erase(this->enemies.begin() + i);
                    this->points += 1.f;
                }   
            }
        }
    } else {
        this->mouseHelds = false;
    }
}


void Game::update() {
    this->pollEvents();

    if (!this->endGame) {
        this->updateMousePosition();
        this->updateEnemies();
    }

    if (this->health == 0) this->endGame = true;
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

