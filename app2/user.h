#pragma once

#include "serializable.h"
#include "console.h"
#include <cstring>
#include <vector>
#include <unordered_map>

class User : public Serializable {
    // Represents all consoles available in the catalog.
    std::unordered_map<int, Console> availableConsoles;
    // IDs of consoles owned by the user.
    std::vector<int> ownedConsoles;
    // Pairs of <console id, game id> for games owned by the user.
    std::vector<std::pair<int, int>> ownedGames;

public:
    User() = default;

    // Catalog methods
    void printAllConsoles() const;
    Console* getConsole(int id);
    const Console* getConsole(int id) const;

    // New user inventory methods
    void printOwnedConsoles(const User& catalog) const;
    void viewInstalledGames(int consoleId, const User& catalog) const;
    bool acquireConsole(int id, const User& catalog);
    bool installGame(int consoleId, int gameId, const User& catalog);
    bool uninstallGame(int consoleId, int gameId);

    // Serialization
    void serialize(std::ofstream& file) const override;
    void deserialize(std::ifstream& file) override;
};
