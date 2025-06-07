#include "console.h"
#include "colors.h"
#include <iostream>

int Console::getId() const {
    return id;
}

void Console::modify(std::string newName, const int storageSpace) {
    this->name = std::move(newName);
    this->storageSpace = storageSpace;
}

Game* Console::getGame(const int id) {
    auto it = games.find(id);
    return (it != games.end()) ? &it->second : nullptr;
}

bool Console::removeGame(const int id) {
    const auto it = games.find(id);
    if (it != games.end()) {
        games.erase(it);
        return true;
    }
    return false;
}

bool Console::addGame(Game& game) {
   if (getGame(game.getId()) != nullptr)
       return false;

    games[game.getId()] = std::move(game);
    return true;
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
    std::cout << id << ". " << COLOR_CYAN << name  << COLOR_RESET << " with " << storageSpace << " GB and " << games.size() << " game(s) available" << std::endl;
}

void Console::printAllGames() const {
    if (games.empty())
        std::cerr << COLOR_RED << "No games found." << COLOR_RESET << std::endl;
    for (const auto& game : games) {
        game.second.printEntry();
    }
}
