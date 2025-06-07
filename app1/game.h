#pragma once

#include "serializable.h"

class Game : public Serializable {
    int id;
    int consoleId;
    int spaceRequired;
    std::string name;
public:
    Game() : id(0), consoleId(0), spaceRequired(0) {}
    Game(const int id, const int consoleId, int spaceRequired, std::string name) : id(id), consoleId(consoleId), spaceRequired(spaceRequired), name(std::move(name)) {}
    void printEntry() const;
    void serialize(std::ofstream& file) const override;
    void deserialize(std::ifstream& file) override;
    int getId() const;
};
