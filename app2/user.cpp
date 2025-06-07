#include "user.h"
#include <iostream>
#include <algorithm>
#include <numeric>
#include "colors.h"

void User::printAllConsoles() const {
    if (availableConsoles.empty()) {
        std::cerr << COLOR_RED << "No consoles found in catalog." << COLOR_RESET << std::endl;
        return;
    }
    for (const auto& pair : availableConsoles) {
        pair.second.printEntry();
    }
}

void User::printOwnedConsoles(const User& catalog) const {
    if (ownedConsoles.empty()) {
        std::cerr << COLOR_RED << "You do not own any consoles." << COLOR_RESET << std::endl;
        return;
    }
    std::cout << "You own the following consoles:" << std::endl;
    for (const int id : ownedConsoles) {
        const Console* console = catalog.getConsole(id);
        if (console) {
            console->printEntry();
        }
    }
}

void User::viewInstalledGames(int consoleId, const User& catalog) const {
    if (std::find(ownedConsoles.begin(), ownedConsoles.end(), consoleId) == ownedConsoles.end()) {
        std::cerr << COLOR_RED << "You do not own a console with this ID." << COLOR_RESET << std::endl;
        return;
    }

    const Console* consoleRef = catalog.getConsole(consoleId);
    if (!consoleRef) return;

    std::cout << "Installed games on " << COLOR_CYAN << consoleRef->getName() << COLOR_RESET << ":" << std::endl;

    int totalSpace = 0;
    int gameCount = 0;
    for (const auto& ownedGame : ownedGames) {
        if (ownedGame.first == consoleId) {
            const Game* game = consoleRef->getGame(ownedGame.second);
            if (game) {
                game->printEntry();
                totalSpace += game->getSpaceRequired();
                gameCount++;
            }
        }
    }

    if (gameCount == 0) {
        std::cerr << COLOR_RED << "No games installed on this console." << COLOR_RESET << std::endl;
    } else {
        std::cout << "Total space used: " << COLOR_YELLOW << totalSpace << " GB" << COLOR_RESET << std::endl;
    }
}

bool User::acquireConsole(int id, const User& catalog) {
    if (!catalog.getConsole(id)) {
        std::cerr << COLOR_RED << "Console with ID " << id << " does not exist in the catalog." << COLOR_RESET << std::endl;
        return false;
    }
    if (std::find(ownedConsoles.begin(), ownedConsoles.end(), id) != ownedConsoles.end()) {
        std::cerr << COLOR_RED << "You already own this console." << COLOR_RESET << std::endl;
        return false;
    }
    ownedConsoles.push_back(id);
    return true;
}

bool User::installGame(int consoleId, int gameId, const User& catalog) {
    if (std::find(ownedConsoles.begin(), ownedConsoles.end(), consoleId) == ownedConsoles.end()) {
        std::cerr << COLOR_RED << "You do not own a console with ID " << consoleId << "." << COLOR_RESET << std::endl;
        return false;
    }

    const Console* console = catalog.getConsole(consoleId);
    if (!console || !console->getGame(gameId)) {
        std::cerr << COLOR_RED << "Game with ID " << gameId << " is not available for this console." << COLOR_RESET << std::endl;
        return false;
    }

    for (const auto& g : ownedGames) {
        if (g.first == consoleId && g.second == gameId) {
            std::cerr << COLOR_RED << "This game is already installed on this console." << COLOR_RESET << std::endl;
            return false;
        }
    }

    ownedGames.emplace_back(consoleId, gameId);
    return true;
}

bool User::uninstallGame(int consoleId, int gameId) {
    auto it = std::remove_if(ownedGames.begin(), ownedGames.end(),
        [consoleId, gameId](const std::pair<int, int>& g) {
            return g.first == consoleId && g.second == gameId;
        });

    if (it != ownedGames.end()) {
        ownedGames.erase(it, ownedGames.end());
        return true;
    }
    
    std::cerr << COLOR_RED << "Game is not installed on this console." << COLOR_RESET << std::endl;
    return false;
}

Console* User::getConsole(int id) {
    auto it = availableConsoles.find(id);
    return (it != availableConsoles.end()) ? &it->second : nullptr;
}

const Console* User::getConsole(int id) const {
    auto it = availableConsoles.find(id);
    return (it != availableConsoles.end()) ? &it->second : nullptr;
}

void User::deserialize(std::ifstream& file) {
    size_t consoleCount, ownedConsoleCount, ownedGameCount;
    file >> consoleCount >> ownedConsoleCount >> ownedGameCount;

    file.get(); 

    availableConsoles.clear();
    availableConsoles.reserve(consoleCount);
    for (size_t i = 0; i < consoleCount; ++i) {
        Console console;
        console.deserialize(file);
        availableConsoles[console.getId()] = std::move(console);
    }

    ownedConsoles.clear();
    ownedConsoles.reserve(ownedConsoleCount);
    for (size_t i = 0; i < ownedConsoleCount; ++i) {
        int id;
        file >> id;
        ownedConsoles.push_back(id);
    }

    ownedGames.clear();
    ownedGames.reserve(ownedGameCount);
    for (size_t i = 0; i < ownedGameCount; ++i) {
        int cId, gId;
        file >> cId >> gId;
        ownedGames.emplace_back(cId, gId);
    }
}

void User::serialize(std::ofstream& file) const {
    file << availableConsoles.size() << " " << ownedConsoles.size() << " " << ownedGames.size() << std::endl;

    for (const auto& pair : availableConsoles) {
        pair.second.serialize(file);
    }
    for (const auto& consoleId : ownedConsoles) {
        file << consoleId << std::endl;
    }
    for (const auto& gamePair : ownedGames) {
        file << gamePair.first << " " << gamePair.second << std::endl;
    }
}
