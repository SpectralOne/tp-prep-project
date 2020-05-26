#include "Game.h"

int main(const int argc, const char **argv) {
    Game game(static_cast<unsigned short>(std::stoul(std::string(argv[1]))));
    game.run();
}
