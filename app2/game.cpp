#include "game.h"
#include <iostream>

void Game::serialize(std::ofstream& file) const {
    file << id << " " << name << " " << consoleId << " " << spaceRequired << std::endl;
}

void Game::deserialize(std::ifstream& file) {
    file >> id >> name >> consoleId >> spaceRequired;
}

void Game::printEntry() const {
    std::cout << "  - " << id << ". " << name << " (" << spaceRequired << " GB)" << std::endl;
}

int Game::getId() const {
    return id;
}

int Game::getSpaceRequired() const {
    return spaceRequired;
}
