#include "user.h"
#include <iostream>

#include "colors.h"

void User::printAllConsoles() const {
    if (availableConsoles.empty())
        std::cerr << COLOR_RED << "No consoles found." << COLOR_RESET << std::endl;
    for (const auto& console : availableConsoles) {
        console.second.printEntry();
    }
}

Console* User::getConsole(const int id) {
    const auto it = availableConsoles.find(id);
    return (it != availableConsoles.end()) ? &it->second : nullptr;
}

bool User::removeConsole(const int id) {
    const auto it = availableConsoles.find(id);
    if (it != availableConsoles.end()) {
        availableConsoles.erase(it);
        return true;
    }
    return false;
}

void User::addConsole(const Console& console) {
    availableConsoles[console.getId()] = console;
}

void User::deserialize(std::ifstream& file) {
    size_t consoleCount, ownedConsoleCount, ownedGameCount;
    file >> consoleCount >> ownedConsoleCount >> ownedGameCount;

    availableConsoles.reserve(consoleCount);
    ownedConsoles.reserve(ownedConsoleCount);
    ownedGames.reserve(ownedGameCount);

    for (size_t i = 0; i < consoleCount; ++i) {
        Console console;
        console.deserialize(file);
        availableConsoles.insert({console.getId(), std::move(console)});
    }

    for (size_t i = 0; i < ownedConsoleCount; ++i) {
        int id; file >> id;
        ownedConsoles.emplace_back(id);
    }

    for (size_t i = 0; i < ownedGameCount; ++i) {
        int consoleId, gameId; file >> consoleId >> gameId;
        ownedGames.emplace_back(consoleId, gameId);
    }
}

void User::serialize(std::ofstream& file) const {
    file << availableConsoles.size() << " " << ownedConsoles.size() << " " << ownedGames.size() << std::endl;

    for (const auto& console : availableConsoles) {
        console.second.serialize(file);
    }

    for (const auto& console : ownedConsoles) {
        file << console << std::endl;
    }

    for (const auto& game : ownedGames) {
        file << game.first << " " << game.second << std::endl;
    }
}

