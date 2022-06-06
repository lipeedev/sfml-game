#include "../include/game.hpp"

int main(int argc, char const *argv[]) {

    std::srand(static_cast<unsigned>(time(NULL)));
    
    Game game;

    while (game.isRunning()) {
        game.update();

        if (game.getEndGame()) game.render(true);
        else game.render(false);
    }

    return 0;
}
