#include "../include/game.hpp"

int main(int argc, char const *argv[]) {

    std::srand(static_cast<unsigned>(time(NULL)));
    
    Game game;

    while (game.isRunning() && !game.getEndGame()) {
        game.update();
        game.render();
    }

    return 0;
}
