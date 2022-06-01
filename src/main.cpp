#include "../include/game.hpp"

int main(int argc, char const *argv[]) {
    
    Game game;

    while (game.isRunning()) {
        game.update();
        game.render();
    }

    return 0;

}
