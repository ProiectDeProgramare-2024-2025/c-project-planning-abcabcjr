#include "console.h"
#include "colors.h"
#include <iostream>

int Console::getId() const {
    return id;
}

std::string Console::getName() const {
    return name;
}

Game* Console::getGame(int id) {
    auto it = games.find(id);
    return (it != games.end()) ? &it->second : nullptr;
}

const Game* Console::getGame(int id) const {
    auto it = games.find(id);
    return (it != games.end()) ? &it->second : nullptr;
}

void Console::serialize(std::ofstream& file) const {
    file << id << " " << name << " " << storageSpace << " " << games.size() << std::endl;
    for (const auto& game : games) {
        game.second.serialize(file);
    }
}

void Console::deserialize(std::ifstream& file) {
    size_t gameCount;
    file >> id >> name >> storageSpace >> gameCount;
    games.reserve(gameCount);

    for (size_t i = 0; i < gameCount; ++i) {
        Game game;
        game.deserialize(file);
        games[game.getId()] = game;
    }
}

void Console::printEntry() const {
    std::cout << id << ". " << COLOR_CYAN << name  << COLOR_RESET << " (" << storageSpace << " GB), " << games.size() << " game(s) in catalog" << std::endl;
}

void Console::printAllGames() const {
    if (games.empty()) {
        std::cerr << COLOR_RED << "No games found in catalog for this console." << COLOR_RESET << std::endl;
        return;
    }
    for (const auto& game : games) {
        game.second.printEntry();
    }
}
