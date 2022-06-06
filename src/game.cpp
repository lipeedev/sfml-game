#include "../include/game.hpp"

void Game::initVariables() {
    this->window = nullptr;

    this->endGame = false;
    this->points = 0;
    this->health = 20;
    this->enemySpawnTimerMax = 20.f;
    this->enemySpawnTimer = this->enemySpawnTimerMax;
    this->maxEnemies = 6;
    this->mouseHelds = false;
}

void Game::initWindow() {
    this->videoMode.width = 800;
    this->videoMode.height = 600;
    this->window = new sf::RenderWindow(this->videoMode, "Game Window", sf::Style::Titlebar | sf::Style::Close);
    this->window->setFramerateLimit(60);
}


void Game::initEnemies() {
    this->enemy.setPosition(10.f, 10.f);
    this->enemy.setSize(sf::Vector2f(100.f, 100.f));
    this->enemy.setScale(0.5f, 0.5f);
    this->enemy.setFillColor(sf::Color::Red);
}


void Game::initFonts() {
    if(!this->font.loadFromFile("fonts/FiraCode-Light.ttf")) {
        std::cout << "ERROR::GAME::INITFONTS::Failed to load font!" << std::endl;
    }
}


void Game::initText() {
    this->uiText.setFont(this->font);
    this->uiText.setCharacterSize(20);
    this->uiText.setString("NONE");
}


void Game::initEndGameText() {
       this->gameOverTxt.setFont(this->font);
       this->gameOverTxt.setPosition(sf::Vector2f(300.f, 250.f));
       this->gameOverTxt.setString("GAME OVER");
}


Game::Game() {
    this->initVariables();
    this->initWindow();
    this->initFonts();
    this->initText();
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

    int type = rand() % 5;

    switch (type) {
        case 0:
            this->enemy.setSize(sf::Vector2f(20.f, 20.f));
            this->enemy.setFillColor(sf::Color::Magenta);
            break;
        case 1:
            this->enemy.setSize(sf::Vector2f(30.f, 30.f));
            this->enemy.setFillColor(sf::Color::Blue);
            break;
        case 2:
            this->enemy.setSize(sf::Vector2f(50.f, 50.f));
            this->enemy.setFillColor(sf::Color::Cyan);
            break;
        case 3:
            this->enemy.setSize(sf::Vector2f(70.f, 70.f));
            this->enemy.setFillColor(sf::Color::Green);
            break;
        case 4:
            this->enemy.setSize(sf::Vector2f(100.f, 100.f));
            this->enemy.setFillColor(sf::Color::Red);
            break;

        default:
            this->enemy.setSize(sf::Vector2f(100.f, 100.f));
            this->enemy.setFillColor(sf::Color::Yellow);
            break;
    }

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
            if (this->health >= 0) this->health -= 1;
        }
    }
     
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (!this->mouseHelds) {

            this->mouseHelds = true;
            bool deleted = false;

            for (int i = 0; i < this->enemies.size() && !deleted; i++) {
                if (this->enemies[i].getGlobalBounds().contains(this->mousePosView)) {

                    if (this->enemies[i].getFillColor() == sf::Color::Magenta) {
                        this->points += 10;
                        this->health += 5;
                    } else if (this->enemies[i].getFillColor() == sf::Color::Blue) {
                        this->points += 7;
                        this->health += 3;
                    } else if (this->enemies[i].getFillColor() == sf::Color::Cyan) {
                        this->points += 5;
                    } else if (this->enemies[i].getFillColor() == sf::Color::Green) {
                        this->points += 3;
                    } else if (this->enemies[i].getFillColor() == sf::Color::Red) {
                        this->points += 1;
                    }

                    deleted = true;
                    this->enemies.erase(this->enemies.begin() + i);
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
        this->updateText();
        this->updateEnemies();
    }

    if (this->health <= 0) this->endGame = true;
}


void Game::renderEnemies(sf::RenderTarget &target) {
     for (auto &e : this->enemies) {
        target.draw(e);
    }
}


void Game::render(bool gameOver) {
    this->window->clear();

    if (gameOver) this->renderEndGame(*this->window);
    else {
        this->renderEnemies(*this->window);
        this->renderText(*this->window);
    }

    this->window->display();
}


void Game::renderText(sf::RenderTarget &target) {
       target.draw(this->uiText);
}

void Game::renderEndGame(sf::RenderTarget &target) {
       this->updateText();
       this->initEndGameText();
       target.draw(this->uiText);
       target.draw(this->gameOverTxt);
}


void Game::updateText() {
    std::stringstream ss;
    ss << "POINTS: " << this->points << std::endl
        << "HEALTH: " << this->health << std::endl;

    this->uiText.setString(ss.str());
}