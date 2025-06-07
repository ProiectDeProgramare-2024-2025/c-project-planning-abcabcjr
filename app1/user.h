#pragma once

#include "serializable.h"
#include "console.h"

class User : public Serializable {
    std::unordered_map<int, Console> availableConsoles;
    std::vector<int> ownedConsoles;
    std::vector<std::pair<int, int>> ownedGames; // pair<console id, game id>
public:
    User() {};
    void printAllConsoles() const;

    Console* getConsole(const int id);
    void addConsole(const Console& console);
    bool removeConsole(const int id);
    void serialize(std::ofstream& file) const override;
    void deserialize(std::ifstream& file) override;
};
